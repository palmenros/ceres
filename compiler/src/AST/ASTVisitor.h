/*
 * Copyright (C) 2023 Pedro Palacios Almendros
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

#ifndef COMPILER_ASTVISITOR_H
#define COMPILER_ASTVISITOR_H

#include "AbstractASTVisitor.h"

namespace Ceres::AST {

    class ASTVisitor : public AbstractASTVisitor {
    public:
        // These functions should be overriden to do the required functionality
        virtual void visitAssignmentExpression(AssignmentExpression &expr) override;
        virtual void visitBinaryOperationExpression(BinaryOperationExpression &expr) override;
        virtual void visitBlockStatement(BlockStatement &stm) override;
        virtual void visitBoolLiteral(BoolLiteral &lit) override;
        virtual void visitCommaExpression(CommaExpression &expr) override;
        virtual void visitCompilationUnit(CompilationUnit &unit) override;
        virtual void visitExpressionStatement(ExpressionStatement &stm) override;
        virtual void visitFloatLiteralExpression(FloatLiteralExpression &expr) override;
        virtual void visitForStatement(ForStatement &stm) override;
        virtual void visitFunctionCallExpression(FunctionCallExpression &expr) override;
        virtual void visitFunctionDefinition(FunctionDefinition &def) override;
        virtual void visitIdentifierExpression(IdentifierExpression &expr) override;
        virtual void visitIfStatement(IfStatement &stm) override;
        virtual void visitIntLiteralExpression(IntLiteralExpression &expr) override;
        virtual void visitPostfixExpression(PostfixExpression &expr) override;
        virtual void visitPrefixExpression(PrefixExpression &expr) override;
        virtual void visitReturnStatement(ReturnStatement &stm) override;
        virtual void visitVariableDeclaration(VariableDeclaration &decl) override;
        virtual void visitVariableDeclarationStatement(VariableDeclarationStatement &stm) override;
        virtual void visitWhileStatement(WhileStatement &stm) override;
    };

}// namespace Ceres::AST

#endif//COMPILER_ASTVISITOR_H
