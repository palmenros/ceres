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

#ifndef COMPILER_ASTSTRINGIFIERVISITOR_H
#define COMPILER_ASTSTRINGIFIERVISITOR_H

#include "AbstractReturningASTVisitor.hpp"
#include <string>

namespace Ceres::AST {

    class ASTStringifierVisitor : public AbstractReturningASTVisitor<std::string> {

        std::string doVisitAssignmentExpression(AssignmentExpression &expr) override;
        std::string doVisitBinaryOperationExpression(BinaryOperationExpression &expr) override;
        std::string doVisitBlockStatement(BlockStatement &stm) override;
        std::string doVisitBoolLiteral(BoolLiteral &lit) override;
        std::string doVisitCommaExpression(CommaExpression &expr) override;
        std::string doVisitCompilationUnit(CompilationUnit &unit) override;
        std::string doVisitExpressionStatement(ExpressionStatement &stm) override;
        std::string doVisitFloatLiteralExpression(FloatLiteralExpression &expr) override;
        std::string doVisitForStatement(ForStatement &stm) override;
        std::string doVisitFunctionCallExpression(FunctionCallExpression &expr) override;
        std::string doVisitFunctionDefinition(FunctionDefinition &def) override;
        std::string doVisitIdentifierExpression(IdentifierExpression &expr) override;
        std::string doVisitIfStatement(IfStatement &stm) override;
        std::string doVisitIntLiteralExpression(IntLiteralExpression &expr) override;
        std::string doVisitPostfixExpression(PostfixExpression &expr) override;
        std::string doVisitPrefixExpression(PrefixExpression &expr) override;
        std::string doVisitReturnStatement(ReturnStatement &stm) override;
        std::string doVisitVariableDeclaration(VariableDeclaration &decl) override;
        std::string doVisitVariableDeclarationStatement(VariableDeclarationStatement &stm) override;
        std::string doVisitWhileStatement(WhileStatement &stm) override;
    };

}// namespace Ceres::AST

#endif//COMPILER_ASTSTRINGIFIERVISITOR_H
