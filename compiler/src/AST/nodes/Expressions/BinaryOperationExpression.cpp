#include "BinaryOperationExpression.h"
#include "../../../Typing/BinaryOperation.h"
#include "../../../utils/log.hpp"
#include "../../AbstractASTVisitor.h"

namespace Ceres::AST {
BinaryOperationExpression::BinaryOperationExpression(SourceSpan const& sourceSpan, std::unique_ptr<Expression>&& left,
    std::unique_ptr<Expression>&& right, Typing::BinaryOperation op, SourceSpan opSpan)
    : Expression(sourceSpan)
    , left(std::move(left))
    , right(std::move(right))
    , op(op)
    , opSpan(opSpan)
{
}

void BinaryOperationExpression::accept(AbstractASTVisitor& visitor) { visitor.visitBinaryOperationExpression(*this); }

std::vector<Node*> BinaryOperationExpression::getChildren() const { return { left.get(), right.get() }; }
} // namespace Ceres::AST
