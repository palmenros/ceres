#include "Expression.h"

namespace Ceres::AST {
Expression::Expression(SourceSpan const& sourceSpan)
    : Node(sourceSpan)
    , type(NotYetInferredType::get(NotYetInferredKind::Expression))
{
}

Expression::Expression(SourceSpan const& sourceSpan, Type* type)
    : Node(sourceSpan)
    , type(type)
{
}
} // namespace Ceres::AST