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

#ifndef COMPILER_RETURNINGASTVISITOR_HPP
#define COMPILER_RETURNINGASTVISITOR_HPP

#include "AbstractReturningASTVisitor.hpp"
#include "../utils/log.hpp"

namespace Ceres::AST {

    // T is the return type of the accept and each visit. T must be non void
    template<class T>
    class ReturningASTVisitor : public AbstractReturningASTVisitor<T> {
    public:

        virtual T visitChildren(Node& node);

        // This function should construct the first initialization for the accu,ulator when visiting children
        virtual T defaultValue();

        // This function should return a way to aggregate the values when calling visitChildren() to multiple children
        // By default, only the last value is preserved and returned to the parent
        virtual T aggregateValues(const T& accumulator, const T& next);

        // Default implementation for visiting children

        T doVisitAssignmentExpression(AssignmentExpression &expr) override;
        T doVisitBinaryOperationExpression(BinaryOperationExpression &expr) override;
        T doVisitBlockStatement(BlockStatement &stm) override;
        T doVisitBoolLiteral(BoolLiteral &lit) override;
        T doVisitCommaExpression(CommaExpression &expr) override;
        T doVisitCompilationUnit(CompilationUnit &unit) override;
        T doVisitExpressionStatement(ExpressionStatement &stm) override;
        T doVisitFloatLiteralExpression(FloatLiteralExpression &expr) override;
        T doVisitForStatement(ForStatement &stm) override;
        T doVisitFunctionCallExpression(FunctionCallExpression &expr) override;
        T doVisitFunctionDefinition(FunctionDefinition &def) override;
        T doVisitIdentifierExpression(IdentifierExpression &expr) override;
        T doVisitIfStatement(IfStatement &stm) override;
        T doVisitIntLiteralExpression(IntLiteralExpression &expr) override;
        T doVisitPostfixExpression(PostfixExpression &expr) override;
        T doVisitPrefixExpression(PrefixExpression &expr) override;
        T doVisitReturnStatement(ReturnStatement &stm) override;
        T doVisitVariableDeclaration(VariableDeclaration &decl) override;
        T doVisitVariableDeclarationStatement(VariableDeclarationStatement &stm) override;
        T doVisitWhileStatement(WhileStatement &stm) override;
    };

    template<class T>
    T ReturningASTVisitor<T>::visitChildren(Node &node) {
        T accumulator = defaultValue();
        for(auto& childrenPtr : node.getChildren()) {
            ASSERT(childrenPtr != nullptr);
            accumulator = aggregateValues(accumulator, this->visit(*childrenPtr));
        }
        return accumulator;
    }

    template<class T>
    T ReturningASTVisitor<T>::defaultValue() {
        return T{};
    }

    template<class T>
    T ReturningASTVisitor<T>::aggregateValues(const T &accumulator, const T &next) {
        return next;
    }

    // Visit all children by default

    template<class T>
    T ReturningASTVisitor<T>::doVisitAssignmentExpression(AssignmentExpression &expr) {
        return visitChildren(expr);
    }

    template<class T>
    T ReturningASTVisitor<T>::doVisitBinaryOperationExpression(BinaryOperationExpression &expr) {
        return visitChildren(expr);
    }

    template<class T>
    T ReturningASTVisitor<T>::doVisitBlockStatement(BlockStatement &stm) {
        return visitChildren(stm);
    }

    template<class T>
    T ReturningASTVisitor<T>::doVisitBoolLiteral(BoolLiteral &lit) {
        return visitChildren(lit);
    }

    template<class T>
    T ReturningASTVisitor<T>::doVisitCommaExpression(CommaExpression &expr) {
        return visitChildren(expr);
    }

    template<class T>
    T ReturningASTVisitor<T>::doVisitCompilationUnit(CompilationUnit &unit) {
        return visitChildren(unit);
    }

    template<class T>
    T ReturningASTVisitor<T>::doVisitExpressionStatement(ExpressionStatement &stm) {
        return visitChildren(stm);
    }

    template<class T>
    T ReturningASTVisitor<T>::doVisitFloatLiteralExpression(FloatLiteralExpression &expr) {
        return visitChildren(expr);
    }

    template<class T>
    T ReturningASTVisitor<T>::doVisitForStatement(ForStatement &stm) {
        return visitChildren(stm);
    }

    template<class T>
    T ReturningASTVisitor<T>::doVisitFunctionCallExpression(FunctionCallExpression &expr) {
        return visitChildren(expr);
    }

    template<class T>
    T ReturningASTVisitor<T>::doVisitFunctionDefinition(FunctionDefinition &def) {
        return visitChildren(def);
    }

    template<class T>
    T ReturningASTVisitor<T>::doVisitIdentifierExpression(IdentifierExpression &expr) {
        return visitChildren(expr);
    }

    template<class T>
    T ReturningASTVisitor<T>::doVisitIfStatement(IfStatement &stm) {
        return visitChildren(stm);
    }

    template<class T>
    T ReturningASTVisitor<T>::doVisitIntLiteralExpression(IntLiteralExpression &expr) {
        return visitChildren(expr);
    }

    template<class T>
    T ReturningASTVisitor<T>::doVisitPostfixExpression(PostfixExpression &expr) {
        return visitChildren(expr);
    }

    template<class T>
    T ReturningASTVisitor<T>::doVisitPrefixExpression(PrefixExpression &expr) {
        return visitChildren(expr);
    }

    template<class T>
    T ReturningASTVisitor<T>::doVisitReturnStatement(ReturnStatement &stm) {
        return visitChildren(stm);
    }

    template<class T>
    T ReturningASTVisitor<T>::doVisitVariableDeclaration(VariableDeclaration &decl) {
        return visitChildren(decl);
    }

    template<class T>
    T ReturningASTVisitor<T>::doVisitVariableDeclarationStatement(VariableDeclarationStatement &stm) {
        return visitChildren(stm);
    }

    template<class T>
    T ReturningASTVisitor<T>::doVisitWhileStatement(WhileStatement &stm) {
        return visitChildren(stm);
    }

}

#endif //COMPILER_RETURNINGASTVISITOR_HPP
