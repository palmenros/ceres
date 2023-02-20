#include "Statement.h"

namespace Ceres::AST {
Statement::Statement(SourceSpan const& sourceSpan)
    : Node(sourceSpan)
{
}
} // namespace Ceres::AST