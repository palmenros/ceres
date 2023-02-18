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

#ifndef COMPILER_BINDINGVISITOR_H
#define COMPILER_BINDINGVISITOR_H

#include "../AST/ASTVisitor.h"
#include "Scope.h"

namespace Ceres::Binding {

using namespace AST;

// TODO: The binding visitor is only the first pass of the binding. It should
// embed all the
//          the declarations in the necessary scopes so the second pass (that I
//          think can and should be combined with type-inference and
//          type-checking) can reference the scopes for the needed symbols.

class BindingVisitor : public ASTVisitor {
public:
    Scope *currentScope = nullptr;

    // Since we don't resolve anything in this pass we just have to add the
    // defined names to the tables
    void visitCompilationUnit(CompilationUnit &unit) override;
    void visitBlockStatement(BlockStatement &stm) override;
    void visitFunctionDefinition(FunctionDefinition &def) override;
    void visitVariableDeclaration(VariableDeclaration &decl) override;
    void visitIdentifierExpression(IdentifierExpression &expr) override;
    void visitAssignmentExpression(AssignmentExpression &expr) override;

    // TODO: think about this
    // void visitForStatement(ForStatement &stm) override;
};
} // namespace Ceres::Binding

#endif // COMPILER_BINDINGVISITOR_H
