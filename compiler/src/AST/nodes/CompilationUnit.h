#ifndef COMPILER_COMPILATIONUNIT_H
#define COMPILER_COMPILATIONUNIT_H

#include "FunctionDeclaration.h"
#include "Node.h"
#include "Statements/FunctionDefinition.h"
#include "Statements/VariableDeclaration.h"
#include <memory>
#include <vector>

namespace Ceres::AST {

class CompilationUnit : public Node {
public:
    std::vector<std::unique_ptr<FunctionDefinition>> functionDefinitions;
    std::vector<std::unique_ptr<FunctionDeclaration>> functionDeclarations;
    std::vector<std::unique_ptr<VariableDeclaration>> globalVariableDeclarations;
    std::optional<Binding::SymbolTableScope> scope;

    CompilationUnit(SourceSpan const& sourceSpan,
        std::vector<std::unique_ptr<FunctionDefinition>>&& functionDefinitions,
        std::vector<std::unique_ptr<FunctionDeclaration>>&& functionDeclarations,
        std::vector<std::unique_ptr<VariableDeclaration>>&& globalVariableDeclarations);

    CompilationUnit(SourceSpan const& sourceSpan);

    void accept(AbstractASTVisitor& visitor) override;

    std::vector<Node*> getChildren() const override;
};

} // namespace Ceres::AST

#endif // COMPILER_COMPILATIONUNIT_H
