#include "IdentifierExpression.h"
#include "../../AbstractASTVisitor.h"

namespace Ceres::AST {
IdentifierExpression::IdentifierExpression(SourceSpan const& sourceSpan, std::string identifier)
    : Expression(sourceSpan)
    , identifier(std::move(identifier))
{
}

void IdentifierExpression::accept(AbstractASTVisitor& visitor) { visitor.visitIdentifierExpression(*this); }

std::vector<Node*> IdentifierExpression::getChildren() const { return {}; }
} // namespace Ceres::AST