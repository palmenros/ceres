#include "IntLiteralExpression.h"
#include "../../AbstractASTVisitor.h"
#include <unordered_map>

namespace Ceres::AST {

IntLiteralExpression::IntLiteralExpression(
    SourceSpan const& sourceSpan, IntLiteralBase base, Type* type, std::string str)
    : Expression(sourceSpan, type)
    , base(base)
    , str(std::move(str))
{
}

void IntLiteralExpression::accept(AbstractASTVisitor& visitor) { visitor.visitIntLiteralExpression(*this); }

std::vector<Node*> IntLiteralExpression::getChildren() const { return {}; }
} // namespace Ceres::AST