#ifndef COMPILER_SOURCESPAN_H
#define COMPILER_SOURCESPAN_H

#include <cstddef>

namespace Ceres {

struct SourceSpan {

private:
    explicit SourceSpan(bool isSpanValid);

public:
    // fileId index as managed by SourceManager
    unsigned fileId = 0;

    // startCharacterIndex and endCharacterIndex start at 0. They represent the
    // index into the source char* pointer both ends are inclusive, that is, the
    // range is represented by [startCharacterIndex, endCharacterIndex]
    size_t startCharacterIndex = 0, endCharacterIndex = 0;

    // Does the span contain valid data? Maybe it doesn't
    bool isSpanValid;

    SourceSpan(unsigned int fileId, size_t startCharacterIndex, size_t endCharacterIndex);

    static SourceSpan createInvalidSpan();
};
} // namespace Ceres

#endif // COMPILER_SOURCESPAN_H
