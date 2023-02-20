#include "FloatLiteralExpression.h"

#include "../../AbstractASTVisitor.h"
#include <utility>

namespace Ceres::AST {

FloatLiteralExpression::FloatLiteralExpression(
    SourceSpan const& sourceSpan, FloatLiteralBase base, Type* type, std::string str)
    : Expression(sourceSpan, type)
    , base(base)
    , str(std::move(str))
{
}

void FloatLiteralExpression::accept(AbstractASTVisitor& visitor) { visitor.visitFloatLiteralExpression(*this); }

std::vector<Node*> FloatLiteralExpression::getChildren() const { return {}; }
} // namespace Ceres::AST