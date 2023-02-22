#include "SymbolDeclaration.h"
#include "../AST/nodes/Statements/FunctionDefinition.h"
#include "../AST/nodes/FunctionDeclaration.h"

namespace Ceres::Binding {
AST::FunctionParameter* getParam(const SymbolDeclaration& symbol)
{
    ASSERT(symbol.getParamIdx().has_value());

    auto* node = dynamic_cast<AST::FunctionDefinition*>(symbol.getDeclarationNode());
    ASSERT(node != nullptr);
    ASSERT(symbol.getParamIdx().value() < node->parameters.size());

    auto* param = &node->parameters[symbol.getParamIdx().value()];

    return param;
}

AST::VariableDeclaration* getVarDecl(const SymbolDeclaration& symbol)
{
    auto* node = dynamic_cast<AST::VariableDeclaration*>(symbol.getDeclarationNode());
    ASSERT(node != nullptr);

    return node;
}

AST::FunctionDefinition* getFunDef(const SymbolDeclaration& symbol)
{
    auto* node = dynamic_cast<AST::FunctionDefinition*>(symbol.getDeclarationNode());
    ASSERT(node != nullptr);

    return node;
}

AST::FunctionDeclaration* getFunDec(const SymbolDeclaration& symbol)
{
    auto* node = dynamic_cast<AST::FunctionDeclaration*>(symbol.getDeclarationNode());
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
    switch(kind) {

    case SymbolDeclarationKind::FunctionDefinition:
        return getFunDef(*this)->functionType;
    case SymbolDeclarationKind::FunctionDeclaration:
        return getFunDec(*this)->functionType;
    case SymbolDeclarationKind::LocalVariableDeclaration:
    case SymbolDeclarationKind::GlobalVariableDeclaration:
        return getVarDecl(*this)->type;
    case SymbolDeclarationKind::FunctionParamDeclaration:
        return getParam(*this)->type;
    case SymbolDeclarationKind::Invalid:
        ASSERT_NOT_REACHED();
    default:
        NOT_IMPLEMENTED();
    }
}

Typing::Constness SymbolDeclaration::getConstness() const
{
    switch(kind) {

    case SymbolDeclarationKind::FunctionDefinition:
    case SymbolDeclarationKind::FunctionDeclaration:
        Log::panic("Trying to check constness of function");
    case SymbolDeclarationKind::LocalVariableDeclaration:
    case SymbolDeclarationKind::GlobalVariableDeclaration:
        return getVarDecl(*this)->constness;
    case SymbolDeclarationKind::FunctionParamDeclaration:
        return getParam(*this)->constness;
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
        return getFunDef(*this)->id;
    case SymbolDeclarationKind::FunctionDeclaration:
        return getFunDec(*this)->functionName;
    case SymbolDeclarationKind::LocalVariableDeclaration:
    case SymbolDeclarationKind::GlobalVariableDeclaration:
        return getVarDecl(*this)->id;
    case SymbolDeclarationKind::FunctionParamDeclaration:
        return getParam(*this)->id;
    case SymbolDeclarationKind::Invalid:
        ASSERT_NOT_REACHED();
    default:
        NOT_IMPLEMENTED();
    }
}


} // namespace Ceres::Binding
