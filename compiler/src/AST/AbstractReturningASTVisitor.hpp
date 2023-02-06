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

#ifndef COMPILER_ABSTRACTRETURNINGASTVISITOR_HPP
#define COMPILER_ABSTRACTRETURNINGASTVISITOR_HPP

#include "AbstractASTVisitor.h"
#include "../utils/log.hpp"

namespace Ceres::AST {

    // T is the return type of the accept and each visit. T must be non void
    template<class T>
    class AbstractReturningASTVisitor : private AbstractASTVisitor {
    private:

        // Stores the return value of the previous called visitor.
        // Do not access, it is moved upon return
        T _result;

        void visitAssignmentExpression(AssignmentExpression &expr) override {_result = doVisitAssignmentExpression(expr);}
        void visitBinaryOperationExpression(BinaryOperationExpression &expr) override {_result = doVisitBinaryOperationExpression(expr);}
        void visitBlockStatement(BlockStatement &stm) override {_result = doVisitBlockStatement(stm);}
        void visitBoolLiteral(BoolLiteral &lit) override {_result = doVisitBoolLiteral(lit);}
        void visitCommaExpression(CommaExpression &expr) override {_result = doVisitCommaExpression(expr);}
        void visitCompilationUnit(CompilationUnit &unit) override {_result = doVisitCompilationUnit(unit);}
        void visitExpressionStatement(ExpressionStatement &stm) override {_result = doVisitExpressionStatement(stm);}
        void visitFloatLiteralExpression(FloatLiteralExpression &expr) override {_result = doVisitFloatLiteralExpression(expr);}
        void visitForStatement(ForStatement &stm) override {_result = doVisitForStatement(stm);}
        void visitFunctionCallExpression(FunctionCallExpression &expr) override {_result = doVisitFunctionCallExpression(expr);}
        void visitFunctionDefinition(FunctionDefinition &def) override {_result = doVisitFunctionDefinition(def);}
        void visitIdentifierExpression(IdentifierExpression &expr) override {_result = doVisitIdentifierExpression(expr);}
        void visitIfStatement(IfStatement &stm) override {_result = doVisitIfStatement(stm);}
        void visitIntLiteralExpression(IntLiteralExpression &expr) override {_result = doVisitIntLiteralExpression(expr);}
        void visitPostfixExpression(PostfixExpression &expr) override {_result = doVisitPostfixExpression(expr);}
        void visitPrefixExpression(PrefixExpression &expr) override {_result = doVisitPrefixExpression(expr);}
        void visitReturnStatement(ReturnStatement &stm) override {_result = doVisitReturnStatement(stm);}
        void visitVariableDeclaration(VariableDeclaration &decl) override {_result = doVisitVariableDeclaration(decl);}
        void visitVariableDeclarationStatement(VariableDeclarationStatement &stm) override {_result = doVisitVariableDeclarationStatement(stm);}
        void visitWhileStatement(WhileStatement &stm) override {_result = doVisitWhileStatement(stm);}

    public:

        T visit(Node& node) {
            node.accept(*this);
            return _result;
        }

        T visitChildren(Node& node) {
            T accumulator = defaultValue();
            for(auto& childrenPtr : node.getChildren()) {
                ASSERT(childrenPtr != nullptr);
                accumulator = aggregateValues(accumulator, this->visit(*childrenPtr));
            }
            return accumulator;
        }

        // This function should construct the first initialization for the accu,ulator when visiting children
        T defaultValue() {
            return T{};
        }

        // This function should return a way to aggregate the values when calling visitChildren() to multiple children
        // By default, only the last value is preserved and returned to the parent
        virtual T aggregateValues(const T& accumulator, const T& next) {
            return next;
        }

        // Functions that the visitor should overload that return the selected type

        virtual T doVisitAssignmentExpression(AssignmentExpression &expr) = 0;
        virtual T doVisitBinaryOperationExpression(BinaryOperationExpression &expr) = 0;
        virtual T doVisitBlockStatement(BlockStatement &stm) = 0;
        virtual T doVisitBoolLiteral(BoolLiteral &lit) = 0;
        virtual T doVisitCommaExpression(CommaExpression &expr) = 0;
        virtual T doVisitCompilationUnit(CompilationUnit &unit) = 0;
        virtual T doVisitExpressionStatement(ExpressionStatement &stm) = 0;
        virtual T doVisitFloatLiteralExpression(FloatLiteralExpression &expr) = 0;
        virtual T doVisitForStatement(ForStatement &stm) = 0;
        virtual T doVisitFunctionCallExpression(FunctionCallExpression &expr) = 0;
        virtual T doVisitFunctionDefinition(FunctionDefinition &def) = 0;
        virtual T doVisitIdentifierExpression(IdentifierExpression &expr) = 0;
        virtual T doVisitIfStatement(IfStatement &stm) = 0;
        virtual T doVisitIntLiteralExpression(IntLiteralExpression &expr) = 0;
        virtual T doVisitPostfixExpression(PostfixExpression &expr) = 0;
        virtual T doVisitPrefixExpression(PrefixExpression &expr) = 0;
        virtual T doVisitReturnStatement(ReturnStatement &stm) = 0;
        virtual T doVisitVariableDeclaration(VariableDeclaration &decl) = 0;
        virtual T doVisitVariableDeclarationStatement(VariableDeclarationStatement &stm) = 0;
        virtual T doVisitWhileStatement(WhileStatement &stm) = 0;
    };

}

#endif //COMPILER_ABSTRACTRETURNINGASTVISITOR_HPP
