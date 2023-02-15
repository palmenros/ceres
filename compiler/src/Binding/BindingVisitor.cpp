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

namespace Ceres::Binding {
    void BindingVisitor::visitCompilationUnit(CompilationUnit &unit) {
        // TODO: fix when we have multiple translation units

        ASSERT(translationUnitScope == nullptr);

        // TODO: add global scope class with no parent
        unit.scope = Scope("Translation", nullptr);
        // TODO: does this panic on None option?
        translationUnitScope = &unit.scope.value();
        currentScope = translationUnitScope;

        ASSERT(translationUnitScope != nullptr);
        ASSERT(currentScope != nullptr);

        for (auto c: unit.getChildren()) { visit(*c); }
    }


    void BindingVisitor::visitBlockStatement(BlockStatement &stm) {
        // TODO: needs a special scopeName?
        stm.scope = Scope("block", currentScope);

        ASSERT(stm.scope.has_value());
        currentScope = &stm.scope.value();

        ASSERT(currentScope != nullptr);

        for (auto c: stm.getChildren()) { visit(*c); }

        currentScope = currentScope->getEnclosingScope();
    }

    void BindingVisitor::visitFunctionDefinition(FunctionDefinition &def) {
        // Put everything, arguments included in block scope
        ASSERT(def.block != nullptr);

        {
            // TODO: Add function to current scope to allow recursion,
            // this should be the same as the translation scope, but its better this
            // way in case we decide to allow nested functions
            SymbolDeclaration fsymbol =
                    SymbolDeclaration(SymbolDeclarationKind::FunctionDeclaration, &def);
            ASSERT(currentScope == translationUnitScope);
            currentScope->define(def.functionName, fsymbol);

            def.block->scope = Scope(def.functionName, currentScope);
        }

        ASSERT(def.block->scope.has_value());
        currentScope = &def.block->scope.value();
        ASSERT(currentScope != nullptr);


        for (const auto &p: def.parameters) {
            // TODO: bind to function node?
            SymbolDeclaration symbol =
                    SymbolDeclaration(SymbolDeclarationKind::LocalVariableDeclaration, &def);
            currentScope->define(p.name, symbol);
        }

        for (auto c: def.block->getChildren()) { visit(*c); }

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

    // TODO: Add scope for compilationUnit
}// namespace Ceres::Binding
