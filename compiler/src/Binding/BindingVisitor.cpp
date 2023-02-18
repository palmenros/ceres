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
    void BindingVisitor::visitCompilationUnit(CompilationUnit &unit) {
        // TODO: fix when we have multiple translation units

        ASSERT(currentScope == nullptr);

        // TODO: add global scope class with no parent
        unit.scope = SymbolTableScope("Translation", nullptr);
        // TODO: does this panic on None option?
        currentScope = &unit.scope.value();

        ASSERT(currentScope != nullptr);

        // Log::info("{}", currentScope->getScopeName());

        visitChildren(unit);
    }


    void BindingVisitor::visitBlockStatement(BlockStatement &stm) {
        // TODO: needs a special scopeName?
        stm.scope = SymbolTableScope("block", currentScope);

        ASSERT(stm.scope.has_value());
        currentScope = &stm.scope.value();

        ASSERT(currentScope != nullptr);

        visitChildren(stm);

        currentScope = currentScope->getEnclosingScope();

        // Log::info("{}", stm.scope->getScopeName());
    }

    void BindingVisitor::visitFunctionDefinition(FunctionDefinition &def) {
        // Put everything, arguments included in block scope
        ASSERT(def.block != nullptr);
        ASSERT(currentScope != nullptr);

        {
            // TODO: Add function to current scope to allow recursion,
            // this should be the same as the translation scope, but its better this
            // way in case we decide to allow nested functions
            SymbolDeclaration fsymbol =
                    SymbolDeclaration(SymbolDeclarationKind::FunctionDeclaration, &def);
            currentScope->define(def.functionName, fsymbol);

            def.block->scope = SymbolTableScope(def.functionName, currentScope);
            currentScope = &def.block->scope.value();

            // Log::info("{}", currentScope->getScopeName());
        }


        for (const auto &p: def.parameters) {
            // TODO: bind to function node?
            SymbolDeclaration symbol =
                    SymbolDeclaration(SymbolDeclarationKind::LocalVariableDeclaration, &def);
            currentScope->define(p.name, symbol);
        }

        visitChildren(*def.block);

        currentScope = currentScope->getEnclosingScope();
    }

    void BindingVisitor::visitVariableDeclaration(VariableDeclaration &decl) {
        SymbolDeclarationKind kind = decl.scope == VariableScope::Global
                                             ? SymbolDeclarationKind::GlobalVariableDeclaration
                                             : SymbolDeclarationKind::LocalVariableDeclaration;

        SymbolDeclaration symbol = SymbolDeclaration(kind, &decl);

        ASSERT(currentScope != nullptr);
        currentScope->define(decl.identifier, symbol);
    }


}// namespace Ceres::Binding
