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
    static char const* getDiagnosticFormatString(Diag diagIdentifier);
    static llvm::SourceMgr::DiagKind getDiagnosticKind(Diag diagIdentifier);

    static unsigned numErrors;
    static unsigned numWarnings;
    static unsigned numRemarks;
    static unsigned numNotes;

    static llvm::SMLoc getSMLocFromSourceSpan(SourceSpan const& span);
    static llvm::SMRange getSMRangeFromSourceSpan(SourceSpan const& span);
    static llvm::SMFixIt getSMFixItFromFixIt(FixItSpan const& fixit);

public:
    template<typename... Args> static void report(SourceSpan const& range, Diag id, Args&&... args)
    {
        report(range, id, {}, {}, std::forward<Args>(args)...);
    }
    template<typename... Args>
    static void report(SourceSpan const& range, Diag id, std::vector<SourceSpan> extraRanges, Args&&... args)
    {
        report(range, id, extraRanges, {}, std::forward<Args>(args)...);
    }

    template<typename... Args> static void report(llvm::SMLoc const& loc, Diag id, Args&&... args)
    {
        std::string msg = fmt::format(getDiagnosticFormatString(id), std::forward<Args>(args)...);
        auto kind = getDiagnosticKind(id);
        auto& srcMgr = SourceManager::get().getLLVMSourceMgr();

        srcMgr.PrintMessage(loc, kind, msg);
    }

    template<typename... Args>
    static void report(SourceSpan const& range, Diag id, std::vector<SourceSpan> const& extraRanges,
        std::vector<FixItSpan> const& fixitRanges, Args&&... args)
    {
        std::string msg = fmt::format(getDiagnosticFormatString(id), std::forward<Args>(args)...);
        auto kind = getDiagnosticKind(id);

        auto& srcMgr = SourceManager::get().getLLVMSourceMgr();

        llvm::SMLoc loc {};
        if (range.isSpanValid) {
            loc = getSMLocFromSourceSpan(range);
        }

        std::vector<llvm::SMRange> smRanges;
        smRanges.reserve(extraRanges.size() + 1);

        if (range.isSpanValid) {
            auto mainRange = getSMRangeFromSourceSpan(range);
            smRanges.push_back(mainRange);
        }

        for (auto const& sourceSpan : extraRanges) {
            smRanges.push_back(getSMRangeFromSourceSpan(sourceSpan));
        }

        std::vector<llvm::SMFixIt> smFixIt;
        smFixIt.reserve(fixitRanges.size());

        for (auto const& fixIt : fixitRanges) {
            smFixIt.push_back(getSMFixItFromFixIt(fixIt));
        }

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

} // namespace Ceres

#endif // COMPILER_DIAGNOSTICS_H
