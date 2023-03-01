#include "Statement.h"

namespace Ceres::AST {
Statement::Statement(SourceSpan const& sourceSpan)
    : Node(sourceSpan)
{
}
bool Statement::isTerminator() const { return false; }
} // namespace Ceres::AST