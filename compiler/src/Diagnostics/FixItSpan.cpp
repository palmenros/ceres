#include "FixItSpan.h"

#include <utility>

namespace Ceres {
FixItSpan::FixItSpan(SourceSpan const& sourceSpan, std::string replacementString)
    : sourceSpan(sourceSpan)
    , replacementString(std::move(replacementString))
{
}
} // namespace Ceres