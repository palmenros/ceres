/*
 * Copyright (C) 2023 Pedro Palacios Almendros, Ricardo Maurizio Paul
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

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
    currentScope->resolve(expr.identifierLHS);
    visitChildren(expr);
}
} // namespace Ceres::Binding
