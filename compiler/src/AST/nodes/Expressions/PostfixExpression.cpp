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

std::string postfixOpToString(PostfixOp op) {
    std::string opStr;

    switch (op) {
    case PostfixOp::PostfixIncrement:
        opStr = "++";
        break;
    case PostfixOp::PostfixDecrement:
        opStr = "--";
        break;
    default:
        NOT_IMPLEMENTED();
    }

    return opStr;
}

} // namespace Ceres::AST