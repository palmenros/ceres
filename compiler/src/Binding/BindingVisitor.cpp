#include "BindingVisitor.h"
#include "Scope.h"
#include "SymbolDeclaration.h"
#include <optional>

namespace Ceres::Binding {
void BindingVisitor::visitCompilationUnit(AST::CompilationUnit& unit)
{
    // TODO: fix when we have multiple translation units
    ASSERT(currentScope == nullptr);

    unit.scope = SymbolTableScope(nullptr);
    currentScope = &unit.scope.value();

    ASSERT(currentScope != nullptr);

    visitChildren(unit);
}

void BindingVisitor::visitBlockStatement(AST::BlockStatement& stm)
{
    stm.scope = SymbolTableScope(currentScope);

    ASSERT(stm.scope.has_value());
    currentScope = &stm.scope.value();

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
    def.block->scope = SymbolTableScope(currentScope);
    currentScope = &def.block->scope.value();

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
    // TODO: Panics on NULL, better return optional
    currentScope->resolve(expr.identifier);
}

void BindingVisitor::visitAssignmentExpression(AST::AssignmentExpression& expr)
{
    auto* identifierExpr = dynamic_cast<AST::IdentifierExpression*>(expr.expressionLHS.get());
    if (identifierExpr == nullptr) {
        Log::panic("LHS of expression is not an identifier");
    }

    currentScope->resolve(identifierExpr->identifier);
    visitChildren(expr);
}
} // namespace Ceres::Binding
