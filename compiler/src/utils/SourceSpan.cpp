#include "SourceSpan.h"

namespace Ceres {

SourceSpan::SourceSpan(bool isSpanValid)
    : isSpanValid(isSpanValid)
{
}

SourceSpan SourceSpan::createInvalidSpan() { return SourceSpan { false }; }
SourceSpan::SourceSpan(unsigned int fileId, size_t startCharacterIndex, size_t endCharacterIndex)
    : fileId(fileId)
    , startCharacterIndex(startCharacterIndex)
    , endCharacterIndex(endCharacterIndex)
    , isSpanValid(true)
{
}
} // namespace Ceres