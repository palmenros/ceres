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

#ifndef COMPILER_ABSTRACTASTVISITOR_H
#define COMPILER_ABSTRACTASTVISITOR_H

#include "nodes/CompilationUnit.h"
#include "nodes/Expressions/AssignmentExpression.h"
#include "nodes/Expressions/BoolLiteralExpression.h"
#include "nodes/Expressions/CommaExpression.h"
#include "nodes/Expressions/FloatLiteralExpression.h"
#include "nodes/Expressions/FunctionCallExpression.h"
#include "nodes/Expressions/IdentifierExpression.h"
#include "nodes/Expressions/IntLiteralExpression.h"
#include "nodes/Expressions/PostfixExpression.h"
#include "nodes/Expressions/PrefixExpression.h"
#include "nodes/Node.h"
#include "nodes/Statements/BlockStatement.h"
#include "nodes/Statements/ExpressionStatement.h"
#include "nodes/Statements/ForStatement.h"
#include "nodes/Statements/IfStatement.h"
#include "nodes/Statements/ReturnStatement.h"
#include "nodes/Statements/VariableDeclarationStatement.h"
#include "nodes/Statements/WhileStatement.h"

namespace Ceres::AST {

    class AbstractASTVisitor {
    public:
        // This function should not be overridden, automatically calls the children
        void visitChildren(Node &node);

        void visit(Node &node);

        virtual void visitAssignmentExpression(AssignmentExpression &expr) = 0;
        virtual void visitBinaryOperationExpression(BinaryOperationExpression &expr) = 0;
        virtual void visitBlockStatement(BlockStatement &stm) = 0;
        virtual void visitBoolLiteral(BoolLiteralExpression &lit) = 0;
        virtual void visitCommaExpression(CommaExpression &expr) = 0;
        virtual void visitCompilationUnit(CompilationUnit &unit) = 0;
        virtual void visitExpressionStatement(ExpressionStatement &stm) = 0;
        virtual void visitFloatLiteralExpression(FloatLiteralExpression &expr) = 0;
        virtual void visitForStatement(ForStatement &stm) = 0;
        virtual void visitFunctionCallExpression(FunctionCallExpression &expr) = 0;
        virtual void visitFunctionDefinition(FunctionDefinition &def) = 0;
        virtual void visitIdentifierExpression(IdentifierExpression &expr) = 0;
        virtual void visitIfStatement(IfStatement &stm) = 0;
        virtual void visitIntLiteralExpression(IntLiteralExpression &expr) = 0;
        virtual void visitPostfixExpression(PostfixExpression &expr) = 0;
        virtual void visitPrefixExpression(PrefixExpression &expr) = 0;
        virtual void visitReturnStatement(ReturnStatement &stm) = 0;
        virtual void visitVariableDeclaration(VariableDeclaration &decl) = 0;
        virtual void visitVariableDeclarationStatement(VariableDeclarationStatement &stm) = 0;
        virtual void visitWhileStatement(WhileStatement &stm) = 0;

    };

}// namespace Ceres::AST

#endif//COMPILER_ABSTRACTASTVISITOR_H
