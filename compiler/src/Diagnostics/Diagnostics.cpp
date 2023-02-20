#include "Diagnostics.h"

namespace Ceres {
unsigned Diagnostics::numErrors = 0;
unsigned Diagnostics::numWarnings = 0;
unsigned Diagnostics::numRemarks = 0;
unsigned Diagnostics::numNotes = 0;

char const* diagnosticTexts[] = {
#define DIAG(identifier, severity, formatString) formatString,
#include "Diagnostics.def"
#undef DIAG
};

llvm::SourceMgr::DiagKind diagnosticsKind[] = {
#define DIAG(identifier, severity, formatString) llvm::SourceMgr::DiagKind::DK_##severity,
#include "Diagnostics.def"
#undef DIAG
};

char const* Diagnostics::getDiagnosticFormatString(Diag diagIdentifier) { return diagnosticTexts[(int)diagIdentifier]; }

llvm::SourceMgr::DiagKind Diagnostics::getDiagnosticKind(Diag diagIdentifier)
{
    return diagnosticsKind[(int)diagIdentifier];
}

llvm::SMLoc Diagnostics::getSMLocFromSourceSpan(SourceSpan const& span)
{
    char const* bufferStart
        = SourceManager::get().getLLVMSourceMgr().getMemoryBuffer((unsigned)span.fileId)->getBufferStart();
    return llvm::SMLoc::getFromPointer(bufferStart + span.startCharacterIndex);
}

llvm::SMRange Diagnostics::getSMRangeFromSourceSpan(SourceSpan const& span)
{
    char const* bufferStart
        = SourceManager::get().getLLVMSourceMgr().getMemoryBuffer((unsigned)span.fileId)->getBufferStart();
    return { llvm::SMLoc::getFromPointer(bufferStart + span.startCharacterIndex),
        llvm::SMLoc::getFromPointer(bufferStart + span.endCharacterIndex + 1) };
}

llvm::SMFixIt Diagnostics::getSMFixItFromFixIt(FixItSpan const& fixit)
{
    return { getSMRangeFromSourceSpan(fixit.sourceSpan), fixit.replacementString };
}
} // namespace Ceres
