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

#include "ASTVisitor.h"
#include "../utils/log.hpp"

namespace Ceres::AST {

    void ASTVisitor::visitAssignmentExpression(AssignmentExpression &expr) {
        visitChildren(expr);
    }

    void ASTVisitor::visitBinaryOperationExpression(BinaryOperationExpression &expr) {
        visitChildren(expr);
    }

    void ASTVisitor::visitBlockStatement(BlockStatement &stm) {
        visitChildren(stm);
    }

    void ASTVisitor::visitBoolLiteral(BoolLiteral &lit) {
        visitChildren(lit);
    }

    void ASTVisitor::visitCommaExpression(CommaExpression &expr) {
        visitChildren(expr);
    }

    void ASTVisitor::visitCompilationUnit(CompilationUnit &unit) {
        visitChildren(unit);
    }

    void ASTVisitor::visitExpressionStatement(ExpressionStatement &stm) {
        visitChildren(stm);
    }

    void ASTVisitor::visitFloatLiteralExpression(FloatLiteralExpression &expr) {
        visitChildren(expr);
    }

    void ASTVisitor::visitForStatement(ForStatement &stm) {
        visitChildren(stm);
    }

    void ASTVisitor::visitFunctionCallExpression(FunctionCallExpression &expr) {
        visitChildren(expr);
    }

    void ASTVisitor::visitFunctionDefinition(FunctionDefinition &def) {
        visitChildren(def);
    }

    void ASTVisitor::visitIdentifierExpression(IdentifierExpression &expr) {
        visitChildren(expr);
    }

    void ASTVisitor::visitIfStatement(IfStatement &stm) {
        visitChildren(stm);
    }

    void ASTVisitor::visitIntLiteralExpression(IntLiteralExpression &expr) {
        // TODO: Maybe avoid visiting children for literal? They do not have any, so it could be an optimization
        visitChildren(expr);
    }

    void ASTVisitor::visitPostfixExpression(PostfixExpression &expr) {
        visitChildren(expr);
    }

    void ASTVisitor::visitPrefixExpression(PrefixExpression &expr) {
        visitChildren(expr);
    }

    void ASTVisitor::visitReturnStatement(ReturnStatement &stm) {
        visitChildren(stm);
    }

    void ASTVisitor::visitVariableDeclaration(VariableDeclaration &decl) {
        visitChildren(decl);
    }

    void ASTVisitor::visitVariableDeclarationStatement(VariableDeclarationStatement &stm) {
        visitChildren(stm);
    }

    void ASTVisitor::visitWhileStatement(WhileStatement &stm) {
        visitChildren(stm);
    }
} // AST