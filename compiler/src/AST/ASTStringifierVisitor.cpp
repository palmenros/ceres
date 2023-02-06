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
        std::string opStr = "";


        if (!expr.binaryOp.has_value()) {
            opStr += "=";
        } else {
            switch (expr.binaryOp.value()) {
                case BinaryOp::Mult:
                    opStr += "*=";
                    break;
                case BinaryOp::Div:
                    opStr += "/=";
                    break;
                case BinaryOp::Modulo:
                    opStr += "%=";
                    break;
                case BinaryOp::Sum:
                    opStr += "+=";
                    break;
                case BinaryOp::Subtraction:
                    opStr += "-=";
                    break;
                case BinaryOp::BitshiftLeft:
                    opStr += "<<=";
                    break;
                case BinaryOp::BitshiftRight:
                    opStr += ">>=";
                    break;
                case BinaryOp::BitwiseAnd:
                    opStr += "&=";
                    break;
                case BinaryOp::BitwiseOr:
                    opStr += "|=";
                    break;
                case BinaryOp::BitwiseXor:
                    opStr += "^=";
                    break;
                default:
                    NOT_IMPLEMENTED();
            }
        }

        return fmt::format("(AssignmentExpression BinOp='{}' LHS='{}' RHS={})",
                           opStr,
                           expr.identifierLHS,
                           visit(*expr.expressionRHS));
    }

    std::string ASTStringifierVisitor::doVisitBinaryOperationExpression(BinaryOperationExpression &expr) {
        std::string opStr = "";

        switch (expr.op) {
            case BinaryOp::Mult:
                opStr += "*";
                break;
            case BinaryOp::Div:
                opStr += "/";
                break;
            case BinaryOp::Modulo:
                opStr += "%";
                break;
            case BinaryOp::Sum:
                opStr += "+";
                break;
            case BinaryOp::Subtraction:
                opStr += "-";
                break;
            case BinaryOp::BitshiftLeft:
                opStr += "<<";
                break;
            case BinaryOp::BitshiftRight:
                opStr += ">>";
                break;
            case BinaryOp::LessOrEqual:
                opStr += "<=";
                break;
            case BinaryOp::GreaterOrEqual:
                opStr += ">=";
                break;
            case BinaryOp::GreaterThan:
                opStr += ">";
                break;
            case BinaryOp::LessThan:
                opStr += "<";
                break;
            case BinaryOp::BitwiseAnd:
                opStr += "&";
                break;
            case BinaryOp::BitwiseOr:
                opStr += "|";
                break;
            case BinaryOp::BitwiseXor:
                opStr += "^";
                break;
            case BinaryOp::Equals:
                opStr += "==";
                break;
            case BinaryOp::NotEquals:
                opStr += "!=";
                break;
            case BinaryOp::LogicalAnd:
                opStr += "&&";
                break;
            case BinaryOp::LogicalOr:
                opStr += "||";
                break;
            default:
                NOT_IMPLEMENTED();
                break;
        }


        return fmt::format("(BinaryExpression BinOp='{}' LHS='{}' RHS={})",
                           opStr,
                           visit(*expr.left),
                           visit(*expr.right));
    }

    std::string ASTStringifierVisitor::doVisitBlockStatement(BlockStatement &block) {
        std::string res = "(Block ";

        bool first = true;

        for (const auto &stm: block.statements) {
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
        std::string res = "(CommaExpression ";
        bool first = true;
        for (auto &childPtr: expr.getChildren()) {
            if (first) {
                first = false;
            } else {
                res += " ";
            }
            res += visit(*childPtr);
        }
        return res + ")";
    }

    std::string ASTStringifierVisitor::doVisitCompilationUnit(CompilationUnit &unit) {
        std::string res = "(CompilationUnit ";
        bool first = true;
        for (auto &childPtr: unit.getChildren()) {
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
        switch (expr.base) {
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
        std::string res = "(ForStatement [";
        bool first = true;

        for (const auto &stm: stm.getChildren()) {
            if (first) {
                first = false;
            } else {
                res += " ";
            }
            res += visit(*stm);
        }

        return res + "])";
    }

    std::string ASTStringifierVisitor::doVisitFunctionCallExpression(FunctionCallExpression &expr) {
        std::string args;
        bool first = true;
        for (auto &arg: expr.arguments) {
            if (first) {
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
        for (const auto &param: def.parameters) {
            std::string paramString = fmt::format("{} {}: {}", param.constness == VariableConstness::NonConst ? "var" : "const", param.name, param.type.toString());
            if (first) {
                first = false;
            } else {
                paramsString += ", ";
            }
            paramsString += paramString;
        }

        return fmt::format("(FunctionDefinition name='{}', params='{}' {})", def.functionName, paramsString, visit(*def.block));
    }

    std::string ASTStringifierVisitor::doVisitIdentifierExpression(IdentifierExpression &expr) {
        return fmt::format("(IdentifierExpression id='{}')", expr.identifier);
    }

    std::string ASTStringifierVisitor::doVisitIfStatement(IfStatement &stm) {
        std::string res = "(IfStatement [";
        bool first = true;

        for (const auto &stm: stm.getChildren()) {
            if (first) {
                first = false;
            } else {
                res += " ";
            }
            res += visit(*stm);
        }

        return res + "])";
    }

    std::string ASTStringifierVisitor::doVisitIntLiteralExpression(IntLiteralExpression &expr) {
        std::string baseString;
        // TODO: move to class toString?
        switch (expr.base) {
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
        switch (expr.type) {
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
        std::string opStr = "";

        switch (expr.op) {
            case PostfixOp::PostfixIncrement:
                opStr += "++";
                break;
            case PostfixOp::PostfixDecrement:
                opStr += "--";
                break;
        }

        return fmt::format("(PostfixExpression op='{}' expr='{}')", opStr, visit(*expr.expr));
    }

    std::string ASTStringifierVisitor::doVisitPrefixExpression(PrefixExpression &expr) {
        std::string opStr = "";

        switch (expr.op) {
            case PrefixOp::PrefixIncrement:
                opStr += "++";
                break;
            case PrefixOp::PrefixDecrement:
                opStr += "--";
                break;
            case PrefixOp::UnaryPlus:
                opStr += "+";
                break;
            case PrefixOp::UnaryMinus:
                opStr += "-";
                break;
            case PrefixOp::UnaryLogicalNot:
                opStr += "!";
                break;
            case PrefixOp::UnaryBitwiseNot:
                opStr += "~";
                break;
        }

        return fmt::format("(PrefixExpression op='{}' expr='{}')", opStr, visit(*expr.expr));
    }


    std::string ASTStringifierVisitor::doVisitReturnStatement(ReturnStatement &stm) {
        return fmt::format("(ReturnStatement expr='{}')", visit(*stm.expr));
    }

    std::string ASTStringifierVisitor::doVisitVariableDeclaration(VariableDeclaration &decl) {
        std::string res = "(VariableDeclaration [";
        bool first = true;

        for (const auto &stm: decl.getChildren()) {
            if (first) {
                first = false;
            } else {
                res += " ";
            }
            res += visit(*stm);
        }

        return res + "])";
    }

    std::string ASTStringifierVisitor::doVisitVariableDeclarationStatement(VariableDeclarationStatement &stm) {
        std::string res = "(VariableDeclarationStatement [";
        bool first = true;

        for (const auto &stm: stm.getChildren()) {
            if (first) {
                first = false;
            } else {
                res += " ";
            }
            res += visit(*stm);
        }

        return res + "])";
    }

    std::string ASTStringifierVisitor::doVisitWhileStatement(WhileStatement &stm) {
        std::string res = "(WhileStatement [";
        bool first = true;

        for (const auto &stm: stm.getChildren()) {
            if (first) {
                first = false;
            } else {
                res += " ";
            }
            res += visit(*stm);
        }

        return res + "])";
    }

}// namespace Ceres::AST