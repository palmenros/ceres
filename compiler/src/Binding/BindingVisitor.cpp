#include "BindingVisitor.h"
#include "../Diagnostics/Diagnostics.h"
#include "Scope.h"
#include "SymbolDeclaration.h"
#include <optional>
#include <spdlog/fmt/bundled/core.h>

namespace Ceres::Binding {
void BindingVisitor::visitCompilationUnit(AST::CompilationUnit& unit)
{
    // TODO: fix when we have multiple translation units, we need to add unresolved symbols to translation unit
    ASSERT(currentScope == nullptr);

    unit.scope = SymbolTableScope(nullptr);
    currentScope = &unit.scope.value();
    auto unresolved = SymbolTableScope(nullptr);

    ASSERT(currentScope != nullptr);

    visitChildren(unit);

    for (auto* const n : unresolvedScope) {
        auto resolved = currentScope->resolve(n->identifier);

        if (!resolved.has_value()) {
            Diagnostics::report(n->sourceSpan, Diag::unresolved_identifier, n->identifier);
            n->decl = {};
        } else {
            n->decl = resolved;
        }
    }

    // Now we set the types of the function definitions and declarations
    for (auto& funDec : unit.functionDeclarations) {
        std::vector<Type*> parameterTypes;
        parameterTypes.reserve(funDec->parameters.size());

        for (auto const& param : funDec->parameters) {
            parameterTypes.push_back(param.type);
        }

        funDec->functionType = FunctionType::get(funDec->returnType, parameterTypes);
    }

    for (auto& funDef : unit.functionDefinitions) {
        std::vector<Type*> parameterTypes;
        parameterTypes.reserve(funDef->parameters.size());

        for (auto const& param : funDef->parameters) {
            parameterTypes.push_back(param.type);
        }

        funDef->functionType = FunctionType::get(funDef->returnType, parameterTypes);
    }
}

void BindingVisitor::visitBlockStatement(AST::BlockStatement& stm)
{
    auto s = SymbolTableScope(currentScope);
    currentScope = &s;

    ASSERT(currentScope != nullptr);

    visitChildren(stm);

    currentScope = currentScope->getEnclosingScope();
}

void BindingVisitor::visitFunctionDefinition(AST::FunctionDefinition& def)
{
    ASSERT(def.block != nullptr);
    ASSERT(currentScope != nullptr);

    // Define function
    auto fsymbol = SymbolDeclaration(SymbolDeclarationKind::FunctionDefinition, &def);
    currentScope->define(def.id, fsymbol);
    currentFunction = &def;

    // Create scope for function body
    auto s = SymbolTableScope(currentScope);
    currentScope = &s;

    // Add all parameters
    for (auto param_idx = 0; param_idx < def.parameters.size(); ++param_idx) {
        SymbolDeclaration symbol = SymbolDeclaration(SymbolDeclarationKind::FunctionParamDeclaration, param_idx, &def);
        currentScope->define(def.parameters[param_idx].id, symbol);
    }

    visitChildren(*def.block);

    currentScope = currentScope->getEnclosingScope();
}

void BindingVisitor::visitFunctionDeclaration(AST::FunctionDeclaration& dec)
{
    ASSERT(currentScope != nullptr);

    // Define function
    auto fsymbol = SymbolDeclaration(SymbolDeclarationKind::FunctionDeclaration, &dec);
    currentScope->define(dec.functionName, fsymbol);
}

void BindingVisitor::visitVariableDeclaration(AST::VariableDeclaration& decl)
{
    auto kind = decl.scope == AST::VariableScope::Global ? SymbolDeclarationKind::GlobalVariableDeclaration
                                                         : SymbolDeclarationKind::LocalVariableDeclaration;

    auto symbol = SymbolDeclaration(kind, &decl);

    ASSERT(currentScope != nullptr);

    currentScope->define(decl.id, symbol);

    visitChildren(decl);
}

void BindingVisitor::visitIdentifierExpression(AST::IdentifierExpression& expr)
{

    auto resolved = currentScope->resolve(expr.identifier);

    if (!resolved.has_value()) {
        // TODO: can be global variable or function call
        unresolvedScope.push_back(&expr);
    } else {
        expr.decl = resolved;
    }

    visitChildren(expr);
}

void BindingVisitor::visitAssignmentExpression(AST::AssignmentExpression& expr)
{
    // TODO: check is LHS
    auto* identifierExpr = dynamic_cast<AST::IdentifierExpression*>(expr.expressionLHS.get());
    if (identifierExpr == nullptr) {
        Log::panic("LHS of expression is not an identifier");
    }

    auto resolved = currentScope->resolve(identifierExpr->identifier);

    if (!resolved.has_value()) {
        unresolvedScope.push_back(identifierExpr);
    } else {
        identifierExpr->decl = resolved;
    }

    visitChildren(expr);
}

void BindingVisitor::visitReturnStatement(AST::ReturnStatement& stm)
{
    auto resolved = currentScope->resolve(currentFunction->id);
    ASSERT(resolved.has_value());
    stm.decl = resolved;
    visitChildren(stm);
}

void BindingVisitor::visitForStatement(AST::ForStatement& stm) { NOT_IMPLEMENTED(); }

} // namespace Ceres::Binding
