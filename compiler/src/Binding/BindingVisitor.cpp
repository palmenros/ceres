#include "BindingVisitor.h"
#include "Scope.h"
#include "SymbolDeclaration.h"
#include <optional>

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
            Log::panic("Unresolved identifier access");
        }
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

    // Define function in global scope
    auto fsymbol = SymbolDeclaration(SymbolDeclarationKind::FunctionDeclaration, &def);
    currentScope->define(def.functionName, fsymbol);

    // Create scope for function body
    auto s = SymbolTableScope(currentScope);
    currentScope = &s;

    // Add all parameters
    for (auto param_idx = 0; param_idx < def.parameters.size(); ++param_idx) {
        SymbolDeclaration symbol = SymbolDeclaration(param_idx, &def);
        currentScope->define(def.parameters[param_idx].name, symbol);
    }

    visitChildren(*def.block);

    currentScope = currentScope->getEnclosingScope();
}

void BindingVisitor::visitVariableDeclaration(AST::VariableDeclaration& decl)
{
    auto kind = decl.scope == AST::VariableScope::Global ? SymbolDeclarationKind::GlobalVariableDeclaration
                                                         : SymbolDeclarationKind::LocalVariableDeclaration;

    auto symbol = SymbolDeclaration(kind, &decl);

    ASSERT(currentScope != nullptr);

    visitChildren(decl);

    currentScope->define(decl.identifier, symbol);
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
} // namespace Ceres::Binding
