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

std::string prefixOpToString(PrefixOp op) {
    std::string opStr;
    switch (op) {
    case PrefixOp::PrefixIncrement:
        opStr = "++";
        break;
    case PrefixOp::PrefixDecrement:
        opStr = "--";
        break;
    case PrefixOp::UnaryPlus:
        opStr = "+";
        break;
    case PrefixOp::UnaryMinus:
        opStr = "-";
        break;
    case PrefixOp::UnaryLogicalNot:
        opStr = "!";
        break;
    case PrefixOp::UnaryBitwiseNot:
        opStr = "~";
        break;
    default:
        NOT_IMPLEMENTED();
    }

    return opStr;
}

} // namespace Ceres::AST