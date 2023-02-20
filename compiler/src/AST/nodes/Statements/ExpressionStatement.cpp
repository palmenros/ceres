#include "ExpressionStatement.h"
#include "../../AbstractASTVisitor.h"

namespace Ceres::AST {
ExpressionStatement::ExpressionStatement(SourceSpan&& sourceSpan, std::unique_ptr<Expression>&& expression)
    : Statement(sourceSpan)
    , expression(std::move(expression))
{
}

void ExpressionStatement::accept(AbstractASTVisitor& visitor) { visitor.visitExpressionStatement(*this); }

std::vector<Node*> ExpressionStatement::getChildren() const { return { expression.get() }; }
} // namespace Ceres::AST