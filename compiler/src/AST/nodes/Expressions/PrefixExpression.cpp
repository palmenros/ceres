#include "PrefixExpression.h"
#include "../../AbstractASTVisitor.h"

namespace Ceres::AST {

PrefixExpression::PrefixExpression(
    SourceSpan const& sourceSpan, PrefixOp op, std::unique_ptr<Expression>&& expr, SourceSpan const& opSourceSpan)
    : Expression(sourceSpan)
    , op(op)
    , expr(std::move(expr))
    , opSourceSpan(opSourceSpan)
{
}

void PrefixExpression::accept(AbstractASTVisitor& visitor) { visitor.visitPrefixExpression(*this); }

std::vector<Node*> PrefixExpression::getChildren() const { return { expr.get() }; }
} // namespace Ceres::AST