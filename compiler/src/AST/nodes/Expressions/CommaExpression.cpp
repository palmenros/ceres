#include "CommaExpression.h"
#include "../../AbstractASTVisitor.h"

namespace Ceres::AST {
CommaExpression::CommaExpression(SourceSpan const& sourceSpan, std::vector<std::unique_ptr<Expression>>&& expressions)
    : Expression(sourceSpan)
    , expressions(std::move(expressions))
{
}

void CommaExpression::accept(AbstractASTVisitor& visitor) { visitor.visitCommaExpression(*this); }

std::vector<Node*> CommaExpression::getChildren() const
{
    std::vector<Node*> v;
    v.reserve(expressions.size());

    for (auto& a : expressions) {
        v.push_back(a.get());
    }
    return v;
}
} // namespace Ceres::AST