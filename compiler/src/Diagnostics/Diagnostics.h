/*
 * Copyright (C) 2023 Pedro Palacios Almendros
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef COMPILER_DIAGNOSTICS_H
#define COMPILER_DIAGNOSTICS_H

#include "../utils/SourceManager.h"
#include "../utils/SourceSpan.h"
#include "FixItSpan.h"
#include "spdlog/fmt/fmt.h"
#include "llvm/Support/SMLoc.h"
#include "llvm/Support/SourceMgr.h"

namespace Ceres {

    enum class Diag {
#define DIAG(identifier, severity, formatString) identifier,
#include "Diagnostics.def"
#undef DIAG
    };

    class Diagnostics {
    protected:
        static const char *getDiagnosticFormatString(Diag diagIdentifier);
        static llvm::SourceMgr::DiagKind getDiagnosticKind(Diag diagIdentifier);

        static unsigned numErrors;
        static unsigned numWarnings;
        static unsigned numRemarks;
        static unsigned numNotes;

        static llvm::SMLoc getSMLocFromSourceSpan(const SourceSpan &span);
        static llvm::SMRange getSMRangeFromSourceSpan(const SourceSpan &span);
        static llvm::SMFixIt getSMFixItFromFixIt(const FixItSpan &fixit);

    public:
        template<typename... Args>
        static void report(const SourceSpan &range, Diag id, Args &&...args) {
            report(range, id, {}, {}, std::forward<Args>(args)...);
        }
        template<typename... Args>
        static void report(const SourceSpan &range, Diag id, std::vector<SourceSpan> extraRanges,
                           Args &&...args) {
            report(range, id, extraRanges, {}, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void report(const llvm::SMLoc &loc, Diag id, Args &&...args) {
            std::string msg =
                    fmt::format(getDiagnosticFormatString(id), std::forward<Args>(args)...);
            auto kind = getDiagnosticKind(id);
            auto &srcMgr = SourceManager::get().getLLVMSourceMgr();

            srcMgr.PrintMessage(loc, kind, msg);
        }

        template<typename... Args>
        static void report(const SourceSpan &range, Diag id,
                           const std::vector<SourceSpan> &extraRanges,
                           const std::vector<FixItSpan> &fixitRanges, Args &&...args) {
            std::string msg =
                    fmt::format(getDiagnosticFormatString(id), std::forward<Args>(args)...);
            auto kind = getDiagnosticKind(id);

            auto &srcMgr = SourceManager::get().getLLVMSourceMgr();

            llvm::SMLoc loc{};
            if (range.isSpanValid) { loc = getSMLocFromSourceSpan(range); }

            std::vector<llvm::SMRange> smRanges;
            smRanges.reserve(extraRanges.size() + 1);

            if (range.isSpanValid) {
                auto mainRange = getSMRangeFromSourceSpan(range);
                smRanges.push_back(mainRange);
            }

            for (const auto &sourceSpan: extraRanges) {
                smRanges.push_back(getSMRangeFromSourceSpan(sourceSpan));
            }

            std::vector<llvm::SMFixIt> smFixIt;
            smFixIt.reserve(fixitRanges.size());

            for (const auto &fixIt: fixitRanges) { smFixIt.push_back(getSMFixItFromFixIt(fixIt)); }

            srcMgr.PrintMessage(loc, kind, msg, smRanges, smFixIt);

            switch (kind) {
                case llvm::SourceMgr::DK_Error:
                    numErrors++;
                    break;
                case llvm::SourceMgr::DK_Warning:
                    numWarnings++;
                    break;
                case llvm::SourceMgr::DK_Remark:
                    numRemarks++;
                    break;
                case llvm::SourceMgr::DK_Note:
                    numNotes++;
                    break;
            }
        }
    };

}// namespace Ceres

#endif//COMPILER_DIAGNOSTICS_H
