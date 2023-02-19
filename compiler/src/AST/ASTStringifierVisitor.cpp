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
#include <iterator>

namespace Ceres::AST {
std::string
ASTStringifierVisitor::doVisitAssignmentExpression(AssignmentExpression &expr) {
    std::string opStr;

    if (!expr.binaryOp.has_value()) {
        opStr = "=";
    } else {
        switch (expr.binaryOp.value()) {
        case BinaryOp::Mult:
            opStr = "*=";
            break;
        case BinaryOp::Div:
            opStr = "/=";
            break;
        case BinaryOp::Modulo:
            opStr = "%=";
            break;
        case BinaryOp::Sum:
            opStr = "+=";
            break;
        case BinaryOp::Subtraction:
            opStr = "-=";
            break;
        case BinaryOp::BitshiftLeft:
            opStr = "<<=";
            break;
        case BinaryOp::BitshiftRight:
            opStr = ">>=";
            break;
        case BinaryOp::BitwiseAnd:
            opStr = "&=";
            break;
        case BinaryOp::BitwiseOr:
            opStr = "|=";
            break;
        case BinaryOp::BitwiseXor:
            opStr = "^=";
            break;
        default:
            NOT_IMPLEMENTED();
        }
    }

    return fmt::format("(AssignmentExpression op='{}' lhs='{}' rhs={})", opStr,
                       expr.identifierLHS, visit(*expr.expressionRHS));
}

std::string ASTStringifierVisitor::doVisitBinaryOperationExpression(
    BinaryOperationExpression &expr) {
    std::string opStr;

    switch (expr.op) {
    case BinaryOp::Mult:
        opStr = "*";
        break;
    case BinaryOp::Div:
        opStr = "/";
        break;
    case BinaryOp::Modulo:
        opStr = "%";
        break;
    case BinaryOp::Sum:
        opStr = "+";
        break;
    case BinaryOp::Subtraction:
        opStr = "-";
        break;
    case BinaryOp::BitshiftLeft:
        opStr = "<<";
        break;
    case BinaryOp::BitshiftRight:
        opStr = ">>";
        break;
    case BinaryOp::LessOrEqual:
        opStr = "<=";
        break;
    case BinaryOp::GreaterOrEqual:
        opStr = ">=";
        break;
    case BinaryOp::GreaterThan:
        opStr = ">";
        break;
    case BinaryOp::LessThan:
        opStr = "<";
        break;
    case BinaryOp::BitwiseAnd:
        opStr = "&";
        break;
    case BinaryOp::BitwiseOr:
        opStr = "|";
        break;
    case BinaryOp::BitwiseXor:
        opStr = "^";
        break;
    case BinaryOp::Equals:
        opStr = "==";
        break;
    case BinaryOp::NotEquals:
        opStr = "!=";
        break;
    case BinaryOp::LogicalAnd:
        opStr = "&&";
        break;
    case BinaryOp::LogicalOr:
        opStr = "||";
        break;
    default:
        NOT_IMPLEMENTED();
        break;
    }

    return fmt::format("(BinaryExpression BinOp='{}' lhs='{}' rhs={})", opStr,
                       visit(*expr.left), visit(*expr.right));
}

std::string
ASTStringifierVisitor::doVisitBlockStatement(BlockStatement &block) {
    std::string res;
    bool first = true;

    for (const auto &stm : block.statements) {
        if (first) {
            first = false;
        } else {
            res += " ";
        }
        res += visit(*stm);
    }

    return fmt::format("(Block '{}')", res);
}

std::string
ASTStringifierVisitor::doVisitBoolLiteral(BoolLiteralExpression &lit) {
    return fmt::format(
        "(BoolLiteralExpression {})",
        BoolLiteralExpression::toStringBoolLiteralValue(lit.value));
}

std::string
ASTStringifierVisitor::doVisitCommaExpression(CommaExpression &expr) {
    std::string res = "(CommaExpression ";
    bool first = true;
    for (auto &childPtr : expr.getChildren()) {
        if (first) {
            first = false;
        } else {
            res += " ";
        }
        res += visit(*childPtr);
    }
    return res + ")";
}

std::string
ASTStringifierVisitor::doVisitCompilationUnit(CompilationUnit &unit) {
    std::string res;
    bool first = true;
    for (auto &childPtr : unit.getChildren()) {
        if (first) {
            first = false;
        } else {
            res += " ";
        }
        res += visit(*childPtr);
    }
    return fmt::format("(CompilationUnit '{}')", res);
}

std::string
ASTStringifierVisitor::doVisitExpressionStatement(ExpressionStatement &stm) {
    return fmt::format("(ExpressionStatement '{}')", visit(*stm.expression));
}

std::string ASTStringifierVisitor::doVisitFloatLiteralExpression(
    FloatLiteralExpression &expr) {
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

    return fmt::format("(FloatLiteral base='{}' str='{}')", baseString,
                       expr.str);
}

std::string ASTStringifierVisitor::doVisitForStatement(ForStatement &stm) {
    std::string res;

    if (stm.maybeInitDeclaration != nullptr) {
        res += " decl='";
        res += visit(*stm.maybeInitDeclaration);
        res += "'";
    }

    if (stm.maybeInitExpression != nullptr) {
        res += " expr='";
        res += visit(*stm.maybeInitExpression);
        res += "'";
    }

    if (stm.maybeConditionExpr != nullptr) {
        res += " cond='";
        res += visit(*stm.maybeConditionExpr);
        res += "'";
    }

    if (stm.maybeUpdateExpr != nullptr) {
        res += " update='";
        res += visit(*stm.maybeUpdateExpr);
        res += "'";
    }

    return fmt::format("(ForStatement{} body='{}')", res, visit(*stm.body));
}

std::string ASTStringifierVisitor::doVisitFunctionCallExpression(
    FunctionCallExpression &expr) {
    std::string args;
    bool first = true;
    for (auto &arg : expr.arguments) {
        if (first) {
            first = false;
        } else {
            args += ", ";
        }
        args += visit(*arg);
    }

    return fmt::format("(FunctionCallExpression id='{}' args='{}')",
                       expr.functionIdentifier, args);
}

std::string
ASTStringifierVisitor::doVisitFunctionDefinition(FunctionDefinition &def) {
    std::string paramsString;
    bool first = true;
    for (const auto &param : def.parameters) {
        std::string paramString = fmt::format(
            "{} {}: {}",
            param.constness == VariableConstness::NonConst ? "var" : "const",
            param.name, param.type->toString());
        if (first) {
            first = false;
        } else {
            paramsString += ", ";
        }
        paramsString += paramString;
    }

    return fmt::format("(FunctionDefinition id='{}', params='{}' body='{}')",
                       def.functionName, paramsString, visit(*def.block));
}

std::string
ASTStringifierVisitor::doVisitIdentifierExpression(IdentifierExpression &expr) {
    return fmt::format("(IdentifierExpression id='{}')", expr.identifier);
}

std::string ASTStringifierVisitor::doVisitIfStatement(IfStatement &stm) {
    if (stm.maybeElseStatement != nullptr) {
        return fmt::format("(IfStatement cond='{}' body='{}' else='{}')",
                           visit(*stm.condition), visit(*stm.thenBlock),
                           visit(*stm.maybeElseStatement));
    }

    return fmt::format("(IfStatement cond='{}' body='{}')",
                       visit(*stm.condition), visit(*stm.thenBlock));
}

std::string
ASTStringifierVisitor::doVisitIntLiteralExpression(IntLiteralExpression &expr) {
    std::string baseString;

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

    std::string typeString = expr.type->toString();

    return fmt::format("(IntLiteralExpression base='{}' text='{}' type='{}')",
                       baseString, expr.str, typeString);
}

std::string
ASTStringifierVisitor::doVisitPostfixExpression(PostfixExpression &expr) {
    std::string opStr;

    switch (expr.op) {
    case PostfixOp::PostfixIncrement:
        opStr = "++";
        break;
    case PostfixOp::PostfixDecrement:
        opStr = "--";
        break;
    }

    return fmt::format("(PostfixExpression op='{}' expr='{}')", opStr,
                       visit(*expr.expr));
}

std::string
ASTStringifierVisitor::doVisitPrefixExpression(PrefixExpression &expr) {
    std::string opStr;

    switch (expr.op) {
    case PrefixOp::PrefixIncrement:
        opStr = "++";
        break;
    case PrefixOp::PrefixDecrement:
        opStr = "--";
        break;
    case PrefixOp::UnaryPlus:
        opStr = "+";
        break;
    case PrefixOp::UnaryMinus:
        opStr = "-";
        break;
    case PrefixOp::UnaryLogicalNot:
        opStr = "!";
        break;
    case PrefixOp::UnaryBitwiseNot:
        opStr = "~";
        break;
    }

    return fmt::format("(PrefixExpression op='{}' expr='{}')", opStr,
                       visit(*expr.expr));
}

std::string
ASTStringifierVisitor::doVisitReturnStatement(ReturnStatement &stm) {
    return fmt::format("(ReturnStatement expr='{}')", visit(*stm.expr));
}

std::string
ASTStringifierVisitor::doVisitVariableDeclaration(VariableDeclaration &decl) {
    std::string vis =
        decl.visibility == VariableVisibility::Private ? "private" : "public";
    std::string scope = decl.scope == VariableScope::Local ? "local" : "global";
    std::string constn =
        decl.constness == VariableConstness::Const ? "const" : "var";

    if (decl.initializerExpression != nullptr) {
        return fmt::format(
            "(VariableDeclaration vis='{}' scope='{}' const='{}' type='{}' "
            "id='{}' expr='{}')",
            vis, scope, constn, decl.type->toString(), decl.identifier,
            visit(*decl.initializerExpression));
    }

    return fmt::format("(VariableDeclaration vis='{}' scope='{}' const='{}' "
                       "type='{}' id='{}')",
                       vis, scope, constn, decl.type->toString(),
                       decl.identifier);
}

std::string ASTStringifierVisitor::doVisitVariableDeclarationStatement(
    VariableDeclarationStatement &stm) {
    return fmt::format("(VariableDeclarationStatement '{}')",
                       visit(*stm.variableDeclaration));
}

std::string ASTStringifierVisitor::doVisitWhileStatement(WhileStatement &stm) {
    return fmt::format("(WhileStatement cond='{}' body='{}')",
                       visit(*stm.condition), visit(*stm.body));
}
} // namespace Ceres::AST