#include "BoolLiteralExpression.h"
#include "../../AbstractASTVisitor.h"

namespace Ceres::AST {
BoolLiteralExpression::BoolLiteralExpression(SourceSpan const& sourceSpan, BoolLiteralValue value)
    : Expression(sourceSpan)
    , value(value)
{
}

void BoolLiteralExpression::accept(AbstractASTVisitor& visitor) { visitor.visitBoolLiteral(*this); }

std::vector<Node*> BoolLiteralExpression::getChildren() const { return {}; }

std::string BoolLiteralExpression::toStringBoolLiteralValue(BoolLiteralValue value)
{
    if (value == BoolLiteralValue::True) {
        return "true";
    } else {
        return "false";
    }
}
} // namespace Ceres::AST