#include "SymbolDeclaration.h"
#include "../AST/nodes/Statements/FunctionDefinition.h"

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

AST::FunctionDefinition* getFunDecl(const SymbolDeclaration& symbol)
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
SymbolDeclarationKind SymbolDeclaration::getKind() const { return kind; }

AST::Node* SymbolDeclaration::getDeclarationNode() const { return declarationNode; }

std::optional<size_t> SymbolDeclaration::getParamIdx() const { return paramIdx; }

Type* SymbolDeclaration::getType() const
{
    if (kind == SymbolDeclarationKind::FunctionParamDeclaration) {
        return getParam(*this)->type;
    }

    if (kind == SymbolDeclarationKind::FunctionDeclaration) {
        return getFunDecl(*this)->returnType;
    }

    return getVarDecl(*this)->type;
}

Typing::Constness SymbolDeclaration::getConstness() const
{
    if (kind == SymbolDeclarationKind::FunctionParamDeclaration) {
        return getParam(*this)->constness;
    }

    if (kind == SymbolDeclarationKind::FunctionDeclaration) {
        Log::panic("Trying to check constness of function");
    }

    return getVarDecl(*this)->constness;
}

std::string SymbolDeclaration::getId()
{
    if (kind == SymbolDeclarationKind::FunctionParamDeclaration) {
        return getParam(*this)->id;
    }

    if (kind == SymbolDeclarationKind::FunctionDeclaration) {
        return getFunDecl(*this)->id;
    }

    return getVarDecl(*this)->id;
}

} // namespace Ceres::Binding
