#ifndef COMPILER_VARIABLEDECLARATIONSTATEMENT_H
#define COMPILER_VARIABLEDECLARATIONSTATEMENT_H

#include "../VariableDeclaration.h"
#include "Statement.h"
#include <memory>

namespace Ceres::AST {

// Class that wraps a Variable Declaration.
// It's intended for operations that should only be done on Variable Declaration
// Statement vs global variable declarations.
class VariableDeclarationStatement : public Statement {
public:
    std::unique_ptr<VariableDeclaration> variableDeclaration;

    VariableDeclarationStatement(
        SourceSpan const& sourceSpan, std::unique_ptr<VariableDeclaration>&& variableDeclaration);

    void accept(AbstractASTVisitor& visitor) override;

    std::vector<Node*> getChildren() const override;
};

} // namespace Ceres::AST

#endif // COMPILER_VARIABLEDECLARATIONSTATEMENT_H
