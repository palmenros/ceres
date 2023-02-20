#include "ForStatement.h"
#include "../../AbstractASTVisitor.h"

namespace Ceres::AST {
ForStatement::ForStatement(SourceSpan const& sourceSpan, std::unique_ptr<VariableDeclaration>&& maybeInitDeclaration,
    std::unique_ptr<Expression>&& maybeInitExpression, std::unique_ptr<Expression>&& conditionExpr,
    std::unique_ptr<Expression>&& updateExpr, std::unique_ptr<BlockStatement>&& body)
    : Statement(sourceSpan)
    , maybeInitDeclaration(std::move(maybeInitDeclaration))
    , maybeInitExpression(std::move(maybeInitExpression))
    , maybeConditionExpr(std::move(conditionExpr))
    , maybeUpdateExpr(std::move(updateExpr))
    , body(std::move(body))
{
}

void ForStatement::accept(AbstractASTVisitor& visitor) { visitor.visitForStatement(*this); }

std::vector<Node*> ForStatement::getChildren() const
{
    std::vector<Node*> v;

    if (maybeInitDeclaration != nullptr) {
        v.push_back(maybeInitDeclaration.get());
    }

    if (maybeInitExpression != nullptr) {
        v.push_back(maybeInitExpression.get());
    }

    if (maybeConditionExpr != nullptr) {
        v.push_back(maybeConditionExpr.get());
    }

    if (maybeUpdateExpr != nullptr) {
        v.push_back(maybeUpdateExpr.get());
    }

    v.push_back(body.get());

    return v;
}
} // namespace Ceres::AST