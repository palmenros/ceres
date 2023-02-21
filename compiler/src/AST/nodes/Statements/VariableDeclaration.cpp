#include "VariableDeclaration.h"

#include <utility>

#include "../../AbstractASTVisitor.h"
#include <utility>

namespace Ceres::AST {
VariableDeclaration::VariableDeclaration(Ceres::SourceSpan const& sourceSpan,
    std::unique_ptr<Expression>&& initializerExpression, Typing::VariableVisibility visibility,
    Typing::Constness constness, VariableScope scope, Type* type, std::string identifier,
    SourceSpan const& typeSourceSpan, SourceSpan const& identifierSourceSpan)
    : Statement(sourceSpan)
    , initializerExpression(std::move(initializerExpression))
    , visibility(visibility)
    , constness(constness)
    , type(type)
    , id(std::move(std::move(identifier)))
    , typeSourceSpan(typeSourceSpan)
    , identifierSourceSpan(identifierSourceSpan)
    , scope(scope)
{
}

void VariableDeclaration::accept(AbstractASTVisitor& visitor) { visitor.visitVariableDeclaration(*this); }

std::vector<Node*> VariableDeclaration::getChildren() const
{
    if (initializerExpression != nullptr) {
        return { initializerExpression.get() };
    }
    return {};
}
} // namespace Ceres::AST
