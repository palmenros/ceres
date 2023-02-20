#ifndef COMPILER_FIXITSPAN_H
#define COMPILER_FIXITSPAN_H

#include "../utils/SourceSpan.h"
#include <string>

namespace Ceres {

class FixItSpan {
public:
    // Span to be replaced
    SourceSpan sourceSpan;

    // What the selected range should be replaced with
    std::string replacementString;
    FixItSpan(SourceSpan const& sourceSpan, std::string replacementString);
};

} // namespace Ceres

#endif // COMPILER_FIXITSPAN_H
