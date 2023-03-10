#ifndef COMPILER_SYMBOLDECLARATION_H
#define COMPILER_SYMBOLDECLARATION_H

#include "../AST/nodes/Node.h"
#include "../AST/nodes/Statements/VariableDeclaration.h"
#include "../Typing/Type.h"
#include <cstddef>
#include <optional>
#include <string>

// Forward declarations
namespace Ceres::AST {

struct FunctionParameter;
class FunctionDefinition;
class FunctionDeclaration;

}

namespace Ceres::Binding {

enum class SymbolDeclarationKind {
    FunctionDefinition,
    FunctionDeclaration,
    LocalVariableDeclaration,
    GlobalVariableDeclaration,
    FunctionParamDeclaration,

    Invalid // For example there has been an error on binding.
};

class SymbolDeclaration {
private:
    SymbolDeclarationKind kind;
    AST::Node* declarationNode;
    std::optional<size_t> paramIdx = 0;

public:
    // TODO: sadly this is not rust so we cant create a constructor for a
    // variant, just pray no one uses this incorrectly
    SymbolDeclaration(SymbolDeclarationKind kind, AST::Node* declarationNode);
    SymbolDeclaration(SymbolDeclarationKind kind, size_t param_idx, AST::Node* declarationNode);

    // Methods
    SymbolDeclarationKind getKind() const;
    AST::Node* getDeclarationNode() const;
    std::optional<size_t> getParamIdx() const;
    Typing::Constness getConstness() const;
    Type* getType() const;
    // TODO: string view
    std::string getId();

    Ceres::AST::FunctionParameter* getParam() const;
    Ceres::AST::VariableDeclaration* getVarDecl() const;
    Ceres::AST::FunctionDefinition* getFunDef() const;
    Ceres::AST::FunctionDeclaration* getFunDec() const;
};

} // namespace Ceres::Binding

#endif // COMPILER_SYMBOLDECLARATION_H
