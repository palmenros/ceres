#include "SymbolDeclaration.h"
#include "../AST/nodes/FunctionDeclaration.h"
#include "../AST/nodes/Statements/FunctionDefinition.h"

namespace Ceres::Binding {

AST::FunctionParameter* SymbolDeclaration::getParam() const
{
    ASSERT(getParamIdx().has_value());

    auto* node = dynamic_cast<AST::FunctionDefinition*>(getDeclarationNode());
    ASSERT(node != nullptr);
    ASSERT(getParamIdx().value() < node->parameters.size());

    auto* param = &node->parameters[getParamIdx().value()];

    return param;
}

AST::VariableDeclaration* SymbolDeclaration::getVarDecl() const
{
    auto* node = dynamic_cast<AST::VariableDeclaration*>(getDeclarationNode());
    ASSERT(node != nullptr);

    return node;
}

AST::FunctionDefinition* SymbolDeclaration::getFunDef() const
{
    auto* node = dynamic_cast<AST::FunctionDefinition*>(getDeclarationNode());
    ASSERT(node != nullptr);

    return node;
}

AST::FunctionDeclaration* SymbolDeclaration::getFunDec() const
{
    auto* node = dynamic_cast<AST::FunctionDeclaration*>(getDeclarationNode());
    ASSERT(node != nullptr);

    return node;
}

// Constructors
SymbolDeclaration::SymbolDeclaration(SymbolDeclarationKind kind, AST::Node* declarationNode)
    : kind(kind)
    , declarationNode(declarationNode) {};

SymbolDeclaration::SymbolDeclaration(SymbolDeclarationKind kind, size_t param_idx, AST::Node* declarationNode)
    : kind(kind)
    , declarationNode(declarationNode)
    , paramIdx(param_idx) {};

// Methods
SymbolDeclarationKind SymbolDeclaration::getKind() const { return kind; }

AST::Node* SymbolDeclaration::getDeclarationNode() const { return declarationNode; }

std::optional<size_t> SymbolDeclaration::getParamIdx() const { return paramIdx; }

Type* SymbolDeclaration::getType() const
{
    switch (kind) {

    case SymbolDeclarationKind::FunctionDefinition:
        return getFunDef()->functionType;
    case SymbolDeclarationKind::FunctionDeclaration:
        return getFunDec()->functionType;
    case SymbolDeclarationKind::LocalVariableDeclaration:
    case SymbolDeclarationKind::GlobalVariableDeclaration:
        return getVarDecl()->type;
    case SymbolDeclarationKind::FunctionParamDeclaration:
        return getParam()->type;
    case SymbolDeclarationKind::Invalid:
        ASSERT_NOT_REACHED();
    default:
        NOT_IMPLEMENTED();
    }
}

Typing::Constness SymbolDeclaration::getConstness() const
{
    switch (kind) {

    case SymbolDeclarationKind::FunctionDefinition:
    case SymbolDeclarationKind::FunctionDeclaration:
        /* Function declarations and definitions are always const */
        return Typing::Constness::Const;
    case SymbolDeclarationKind::LocalVariableDeclaration:
    case SymbolDeclarationKind::GlobalVariableDeclaration:
        return getVarDecl()->constness;
    case SymbolDeclarationKind::FunctionParamDeclaration:
        return getParam()->constness;
    case SymbolDeclarationKind::Invalid:
        ASSERT_NOT_REACHED();
    default:
        NOT_IMPLEMENTED();
    }
}

std::string SymbolDeclaration::getId()
{
    switch (kind) {

    case SymbolDeclarationKind::FunctionDefinition:
        return getFunDef()->id;
    case SymbolDeclarationKind::FunctionDeclaration:
        return getFunDec()->functionName;
    case SymbolDeclarationKind::LocalVariableDeclaration:
    case SymbolDeclarationKind::GlobalVariableDeclaration:
        return getVarDecl()->id;
    case SymbolDeclarationKind::FunctionParamDeclaration:
        return getParam()->id;
    case SymbolDeclarationKind::Invalid:
        ASSERT_NOT_REACHED();
    default:
        NOT_IMPLEMENTED();
    }
}

} // namespace Ceres::Binding
