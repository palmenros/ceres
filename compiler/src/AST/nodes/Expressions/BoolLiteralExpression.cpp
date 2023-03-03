#include "BoolLiteralExpression.h"
#include "../../AbstractASTVisitor.h"

namespace Ceres::AST {
BoolLiteralExpression::BoolLiteralExpression(SourceSpan const& sourceSpan, BoolLiteralValue value)
    : Expression(sourceSpan, BoolType::get())
    , value(value)
{
}

void BoolLiteralExpression::accept(AbstractASTVisitor& visitor) { visitor.visitBoolLiteral(*this); }

std::vector<Node*> BoolLiteralExpression::getChildren() const { return {}; }

std::string BoolLiteralExpression::toStringBoolLiteralValue(BoolLiteralValue value)
{
    switch (value) {
    case BoolLiteralValue::True:
        return "true";
    case BoolLiteralValue::False:
        return "false";
    default:
        ASSERT_NOT_REACHED();
    }
}

bool BoolLiteralExpression::getLiteralBool() const
{
    switch (value) {
    case BoolLiteralValue::True:
        return true;
    case BoolLiteralValue::False:
        return false;
    default:
        ASSERT_NOT_REACHED();
    }
}

} // namespace Ceres::AST