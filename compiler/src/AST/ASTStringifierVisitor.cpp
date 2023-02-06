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

#include "ASTStringifierVisitor.h"
#include "../utils/log.hpp"
#include "spdlog/fmt/fmt.h"

namespace Ceres::AST {

    std::string ASTStringifierVisitor::doVisitAssignmentExpression(AssignmentExpression &expr) {
        return fmt::format("(AssignmentExpression BinOp='{}' LHS='{}' RHS={})",
                           AssignmentExpression::assignmentBinaryOpToString(expr.binaryOp),
                           expr.identifierLHS,
                           visit(*expr.expressionRHS));
    }

    std::string ASTStringifierVisitor::doVisitBinaryOperationExpression(BinaryOperationExpression &expr) {
        TODO();
    }

    std::string ASTStringifierVisitor::doVisitBlockStatement(BlockStatement &block) {
        std::string res = "(Block ";

        bool first = true;

        for(const auto& stm : block.statements) {
            if (first) {
                first = false;
            } else {
                res += " ";
            }
            res += visit(*stm);
        }

        return res + ")";
    }

    std::string ASTStringifierVisitor::doVisitBoolLiteral(BoolLiteral &lit) {
        return fmt::format("(BoolLiteral {})", BoolLiteral::toStringBoolLiteralValue(lit.value));
    }

    std::string ASTStringifierVisitor::doVisitCommaExpression(CommaExpression &expr) {
        TODO();
    }

    std::string ASTStringifierVisitor::doVisitCompilationUnit(CompilationUnit &unit) {
        std::string res = "(CompilationUnit ";
        bool first = true;
        for (auto& childPtr : unit.getChildren()) {
            if (first) {
                first = false;
            } else {
                res += " ";
            }
            res += visit(*childPtr);
        }
        return res + ")";
    }

    std::string ASTStringifierVisitor::doVisitExpressionStatement(ExpressionStatement &stm) {
        return fmt::format("(ExpressionStatement {})", visit(*stm.expression));
    }

    std::string ASTStringifierVisitor::doVisitFloatLiteralExpression(FloatLiteralExpression &expr) {
        std::string baseString;
        switch(expr.base) {
            case FloatLiteralBase::Dec:
                baseString = "dec";
                break;
            case FloatLiteralBase::Hex:
                baseString = "hex";
                break;
            default:
                NOT_IMPLEMENTED();
        }
        return fmt::format("(FloatLiteral base='{}' str='{}')", baseString, expr.str);
    }

    std::string ASTStringifierVisitor::doVisitForStatement(ForStatement &stm) {
        TODO();
    }

    std::string ASTStringifierVisitor::doVisitFunctionCallExpression(FunctionCallExpression &expr) {
        std::string args;
        bool first = true;
        for (auto& arg : expr.arguments) {
            if(first) {
                first = false;
            } else {
                args += ", ";
            }
            args += visit(*arg);
        }

        return fmt::format("(FunctionCallExpression function_name='{}' args=[{}])", expr.functionIdentifier, args);
    }

    std::string ASTStringifierVisitor::doVisitFunctionDefinition(FunctionDefinition &def) {
        std::string paramsString;
        bool first = true;
        for(const auto& param : def.parameters) {
            std::string paramString = fmt::format("{} {}: {}", param.constness == VariableConstness::NonConst ? "var" : "const", param.name, param.type.toString());
            if(first) {
                first = false;
            } else {
                paramsString += ", ";
            }
            paramsString += paramString;
        }

        return fmt::format("(FunctionDefinition[name='{}', params='{}'] {})", def.functionName, paramsString, visit(*def.block));
    }

    std::string ASTStringifierVisitor::doVisitIdentifierExpression(IdentifierExpression &expr) {
        return fmt::format("(IdentifierExpression id='{}')", expr.identifier);
    }

    std::string ASTStringifierVisitor::doVisitIfStatement(IfStatement &stm) {
        TODO();
    }

    std::string ASTStringifierVisitor::doVisitIntLiteralExpression(IntLiteralExpression &expr) {
        std::string baseString;
        switch(expr.base) {
            case IntLiteralBase::Dec:
                baseString = "dec";
                break;
            case IntLiteralBase::Hex:
                baseString = "hex";
                break;
            case IntLiteralBase::Oct:
                baseString = "oct";
                break;
            case IntLiteralBase::Bin:
                baseString = "bin";
                break;
            default:
                NOT_IMPLEMENTED();
        }

        std::string typeString;
        switch(expr.type) {
            case IntLiteralType::None:
                typeString = "$UNKNOWN";
                break;
            case IntLiteralType::U8:
                typeString = "u8";
                break;
            case IntLiteralType::U16:
                typeString = "u16";
                break;
            case IntLiteralType::U32:
                typeString = "u32";
                break;
            case IntLiteralType::U64:
                typeString = "u64";
                break;
            case IntLiteralType::I8:
                typeString = "i8";
                break;
            case IntLiteralType::I16:
                typeString = "i16";
                break;
            case IntLiteralType::I32:
                typeString = "i32";
                break;
            case IntLiteralType::I64:
                typeString = "i64";
                break;
            default:
                NOT_IMPLEMENTED();
        }

        return fmt::format("(IntLiteralExpression base='{}' text='{}' type='{}')", baseString, expr.str, typeString);
    }

    std::string ASTStringifierVisitor::doVisitPostfixExpression(PostfixExpression &expr) {
        TODO();
    }

    std::string ASTStringifierVisitor::doVisitPrefixExpression(PrefixExpression &expr) {
        TODO();
    }

    std::string ASTStringifierVisitor::doVisitReturnStatement(ReturnStatement &stm) {
        return fmt::format("(ReturnStatement {})", visit(*stm.expr));
    }

    std::string ASTStringifierVisitor::doVisitVariableDeclaration(VariableDeclaration &decl) {
        TODO();
    }

    std::string ASTStringifierVisitor::doVisitVariableDeclarationStatement(VariableDeclarationStatement &stm) {
        TODO();
    }

    std::string ASTStringifierVisitor::doVisitWhileStatement(WhileStatement &stm) {
        TODO();
    }

} // AST