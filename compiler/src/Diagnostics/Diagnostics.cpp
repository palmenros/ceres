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

#include "Diagnostics.h"

namespace Ceres {

    unsigned Diagnostics::numErrors = 0;
    unsigned Diagnostics::numWarnings = 0;
    unsigned Diagnostics::numRemarks = 0;
    unsigned Diagnostics::numNotes = 0;

    const char *diagnosticTexts[] = {
#define DIAG(identifier, severity, formatString) formatString,
#include "Diagnostics.def"
#undef DIAG
    };

    llvm::SourceMgr::DiagKind diagnosticsKind[] = {
#define DIAG(identifier, severity, formatString) llvm::SourceMgr::DiagKind::DK_##severity,
#include "Diagnostics.def"
#undef DIAG
    };

    const char *Diagnostics::getDiagnosticFormatString(Diag diagIdentifier) {
        return diagnosticTexts[(int) diagIdentifier];
    }

    llvm::SourceMgr::DiagKind Diagnostics::getDiagnosticKind(Diag diagIdentifier) {
        return diagnosticsKind[(int) diagIdentifier];
    }

    llvm::SMLoc Diagnostics::getSMLocFromSourceSpan(const SourceSpan &span) {
        const char *bufferStart = SourceManager::get().getLLVMSourceMgr().getMemoryBuffer((unsigned) span.fileId)->getBufferStart();
        return llvm::SMLoc::getFromPointer(bufferStart + span.startCharacterIndex);
    }

    llvm::SMRange Diagnostics::getSMRangeFromSourceSpan(const SourceSpan &span) {
        const char *bufferStart = SourceManager::get().getLLVMSourceMgr().getMemoryBuffer((unsigned) span.fileId)->getBufferStart();
        return {llvm::SMLoc::getFromPointer(bufferStart + span.startCharacterIndex),
                llvm::SMLoc::getFromPointer(bufferStart + span.endCharacterIndex + 1)};
    }

    llvm::SMFixIt Diagnostics::getSMFixItFromFixIt(const FixItSpan &fixit) {
        return {getSMRangeFromSourceSpan(fixit.sourceSpan), fixit.replacementString};
    }
}// namespace Ceres