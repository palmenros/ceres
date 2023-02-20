#include "SymbolDeclaration.h"
#include "../AST/nodes/FunctionDefinition.h"

namespace Ceres::Binding {
AST::FunctionParameter* getParam(SymbolDeclaration& symbol)
{
    ASSERT(symbol.getParamIdx().has_value());

    auto* node = dynamic_cast<AST::FunctionDefinition*>(symbol.getDeclarationNode());
    ASSERT(node != nullptr);
    ASSERT(symbol.getParamIdx().value() < node->parameters.size());

    auto* param = &node->parameters[symbol.getParamIdx().value()];

    return param;
}

AST::VariableDeclaration* getVarDecl(SymbolDeclaration& symbol)
{
    auto* node = dynamic_cast<AST::VariableDeclaration*>(symbol.getDeclarationNode());
    ASSERT(node != nullptr);

    return node;
}

AST::FunctionDefinition* getFunDecl(SymbolDeclaration& symbol)
{
    auto* node = dynamic_cast<AST::FunctionDefinition*>(symbol.getDeclarationNode());
    ASSERT(node != nullptr);

    return node;
}

// Constructors
SymbolDeclaration::SymbolDeclaration(SymbolDeclarationKind kind, AST::Node* declarationNode)
    : kind(kind)
    , declarationNode(declarationNode) {};

SymbolDeclaration::SymbolDeclaration(size_t param_idx, AST::Node* declarationNode)
    : kind(SymbolDeclarationKind::FunctionParamDeclaration)
    , declarationNode(declarationNode)
    , paramIdx(param_idx) {};

// Methods
SymbolDeclarationKind SymbolDeclaration::getKind() { return kind; }

AST::Node* SymbolDeclaration::getDeclarationNode() { return declarationNode; }

std::optional<size_t> SymbolDeclaration::getParamIdx() { return paramIdx; }

Type* SymbolDeclaration::getType()
{
    if (kind == SymbolDeclarationKind::FunctionParamDeclaration) {
        return getParam(*this)->type;
    }

    if (kind == SymbolDeclarationKind::FunctionDeclaration) {
        return getFunDecl(*this)->returnType;
    }

    return getVarDecl(*this)->type;
}

AST::VariableConstness SymbolDeclaration::getConstness()
{
    if (kind == SymbolDeclarationKind::FunctionParamDeclaration) {
        return getParam(*this)->constness;
    }

    if (kind == SymbolDeclarationKind::FunctionDeclaration) {
        Log::panic("Trying to check constness of function");
    }

    return getVarDecl(*this)->constness;
}

} // namespace Ceres::Binding
