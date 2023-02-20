#include "VariableDeclarationStatement.h"
#include "../../AbstractASTVisitor.h"

namespace Ceres::AST {
VariableDeclarationStatement::VariableDeclarationStatement(
    SourceSpan const& sourceSpan, std::unique_ptr<VariableDeclaration>&& variableDeclaration)
    : Statement(sourceSpan)
    , variableDeclaration(std::move(variableDeclaration))
{
}

void VariableDeclarationStatement::accept(AbstractASTVisitor& visitor)
{
    visitor.visitVariableDeclarationStatement(*this);
}

std::vector<Node*> VariableDeclarationStatement::getChildren() const { return { variableDeclaration.get() }; }
} // namespace Ceres::AST