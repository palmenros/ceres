#include "CompilationUnit.h"
#include "../AbstractASTVisitor.h"

namespace Ceres::AST {
CompilationUnit::CompilationUnit(SourceSpan const& sourceSpan,
    std::vector<std::unique_ptr<FunctionDefinition>>&& functionDefinitions,
    std::vector<std::unique_ptr<FunctionDeclaration>>&& functionDeclarations,
    std::vector<std::unique_ptr<VariableDeclaration>>&& globalVariableDeclarations)
    : Node(sourceSpan)
    , functionDefinitions(std::move(functionDefinitions))
    , functionDeclarations(std::move(functionDeclarations))
    , globalVariableDeclarations(std::move(globalVariableDeclarations))
{
}

void CompilationUnit::accept(AbstractASTVisitor& visitor) { visitor.visitCompilationUnit(*this); }

std::vector<Node*> CompilationUnit::getChildren() const
{
    std::vector<Node*> v;
    v.reserve(functionDefinitions.size() + functionDeclarations.size() + globalVariableDeclarations.size());

    for (const auto& ptr : functionDefinitions) {
        v.push_back(ptr.get());
    }

    for(const auto& ptr : functionDeclarations) {
        v.push_back(ptr.get());
    }

    for (const auto& ptr : globalVariableDeclarations) {
        v.push_back(ptr.get());
    }
    return v;
}

CompilationUnit::CompilationUnit(SourceSpan const& sourceSpan)
    : Node(sourceSpan)
{
}
} // namespace Ceres::AST