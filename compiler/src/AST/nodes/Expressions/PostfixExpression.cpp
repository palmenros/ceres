#include "PostfixExpression.h"
#include "../../AbstractASTVisitor.h"

namespace Ceres::AST {
PostfixExpression::PostfixExpression(
    SourceSpan const& sourceSpan, PostfixOp op, std::unique_ptr<Expression>&& expr, SourceSpan opSourceSpan)
    : Expression(sourceSpan)
    , op(op)
    , expr(std::move(expr))
    , opSourceSpan(opSourceSpan)
{
}

void PostfixExpression::accept(AbstractASTVisitor& visitor) { visitor.visitPostfixExpression(*this); }

std::vector<Node*> PostfixExpression::getChildren() const { return { expr.get() }; }
} // namespace Ceres::AST