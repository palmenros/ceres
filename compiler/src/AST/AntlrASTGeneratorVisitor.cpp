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

#include <optional>
#include "AntlrASTGeneratorVisitor.h"
#include "../utils/log.hpp"
#include "FunctionDefinition.h"
#include "VariableDeclaration.h"
#include "CompilationUnit.h"
#include "Type.h"
#include "CommaExpression.h"
#include "IntLiteralExpression.h"
#include "FloatLiteralExpression.h"
#include "BoolLiteral.h"
#include "IdentifierExpression.h"
#include "BinaryOperationExpression.h"
#include "CeresLexer.h"
#include "PostfixExpression.h"
#include "PrefixExpression.h"
#include "FunctionCallExpression.h"
#include "AssignmentExpression.h"

using namespace antlrgenerated;

namespace Ceres::AST {

    std::any AntlrASTGeneratorVisitor::visitErrorNode(antlr4::tree::ErrorNode *node) {
        NOT_IMPLEMENTED();
    }

    SourceSpan AntlrASTGeneratorVisitor::getSourceSpan(const antlr4::ParserRuleContext& context) {
        auto startToken = context.getStart();
        auto endToken = context.getStop();

        ASSERT(startToken != nullptr);
        ASSERT(endToken != nullptr);

        return getSourceSpan(startToken, endToken);
    }

    SourceSpan AntlrASTGeneratorVisitor::getSourceSpan(const antlr4::tree::TerminalNode& context) {
        auto tok = context.getSymbol();

        ASSERT(tok != nullptr);

        return getSourceSpan(tok);
    }

    SourceSpan AntlrASTGeneratorVisitor::getSourceSpan(const antlr4::tree::TerminalNode& start, const antlr4::tree::TerminalNode& end) {
        auto startTok = start.getSymbol();
        auto endTok = end.getSymbol();

        ASSERT(startTok != nullptr);
        ASSERT(endTok != nullptr);

        return getSourceSpan(startTok, endTok);
    }

    SourceSpan AntlrASTGeneratorVisitor::getSourceSpan(const antlr4::Token* tok) {
        return {tok->getLine(), tok->getLine(), tok->getStartIndex(), tok->getStopIndex()};
    }

    SourceSpan AntlrASTGeneratorVisitor::getSourceSpan(const antlr4::Token* startToken, const antlr4::Token* endToken) {
        return {startToken->getLine(), endToken->getLine(), startToken->getStartIndex(), endToken->getStopIndex()};
    }

    std::any AntlrASTGeneratorVisitor::visitCompilationUnit(CeresParser::CompilationUnitContext *ctx) {

        std::vector<std::unique_ptr<FunctionDefinition>> functionDefinitions;
        std::vector<std::unique_ptr<VariableDeclaration>> variableDeclarations;

        ASSERT(ctx != nullptr);

        functionDefinitions.reserve(ctx->functionDefinition().size());

        for(auto funDefinitionContextPtr : ctx->functionDefinition()) {
            auto res = visit(funDefinitionContextPtr);
            auto funDef = std::any_cast<FunctionDefinition*>(res);

            ASSERT(funDef != nullptr);

            functionDefinitions.push_back(std::unique_ptr<FunctionDefinition>(funDef));
        }

        variableDeclarations.reserve(ctx->globalVarDeclaration().size());

        for (auto varDefinitionContextPtr : ctx->globalVarDeclaration()) {
            auto res = visit(varDefinitionContextPtr);
            auto varDef = std::any_cast<VariableDeclaration*>(res);

            ASSERT(varDef != nullptr);

            variableDeclarations.push_back(std::unique_ptr<VariableDeclaration>(varDef));
        }

        return new CompilationUnit(getSourceSpan(*ctx),
                                          std::move(functionDefinitions),
                                          std::move(variableDeclarations));
    }

    std::any AntlrASTGeneratorVisitor::visitGlobalVarDeclaration(CeresParser::GlobalVarDeclarationContext *ctx) {
        ASSERT(ctx != nullptr);

        auto varDeclaration = std::any_cast<VariableDeclaration*>(visit(ctx->varDeclaration()));
        ASSERT(varDeclaration != nullptr);

        if (ctx->PUB() != nullptr) {
            // Pub variable
            varDeclaration->visibility = VariableVisibility::Public;
        } else {
            varDeclaration->visibility = VariableVisibility::Private;
        }

        varDeclaration->scope = VariableScope::Global;
        varDeclaration->sourceSpan = getSourceSpan(*ctx);

        return varDeclaration;
    }

    std::any AntlrASTGeneratorVisitor::visitFunctionDefinition(CeresParser::FunctionDefinitionContext *ctx) {
        NOT_IMPLEMENTED();
    }

    std::any AntlrASTGeneratorVisitor::visitFormalParameters(CeresParser::FormalParametersContext *ctx) {
        NOT_IMPLEMENTED();
    }

    std::any AntlrASTGeneratorVisitor::visitParameter(CeresParser::ParameterContext *ctx) {
        NOT_IMPLEMENTED();
    }

    std::any AntlrASTGeneratorVisitor::visitBlock(CeresParser::BlockContext *ctx) {
        NOT_IMPLEMENTED();
    }

    std::any AntlrASTGeneratorVisitor::visitType(CeresParser::TypeContext *ctx) {
        ASSERT(ctx != nullptr);

        std::string type_text;

        return Type{ctx->getText()};
    }

    std::any AntlrASTGeneratorVisitor::visitVarDeclaration(CeresParser::VarDeclarationContext *ctx) {
        ASSERT(ctx != nullptr);

        ASSERT(ctx->IDENTIFIER() != nullptr);

        std::unique_ptr<Expression> initializer_expression = nullptr;
        if(ctx->expression() != nullptr) {
            auto expr = std::any_cast<Expression*>(visit(ctx->expression()));
            initializer_expression.reset(expr);
        }

        VariableConstness constness;
        if(ctx->VAR() != nullptr) {
            constness = VariableConstness::NonConst;
        } else if (ctx->CONSTANT() != nullptr) {
            constness = VariableConstness::Const;
        } else {
            NOT_IMPLEMENTED();
        }

        Type type = Type::createUnspecifiedType();
        SourceSpan typeSourceSpan = SourceSpan::createInvalidSpan();
        if(ctx->type() != nullptr) {
            type = std::any_cast<Type>(visit(ctx->type()));
            typeSourceSpan = getSourceSpan(*ctx->type());
        }

        return new VariableDeclaration(getSourceSpan(*ctx), std::move(initializer_expression),
                                       VariableVisibility::Private,
                                       constness, VariableScope::Local, std::move(type), ctx->IDENTIFIER()->getText(),
                                       typeSourceSpan,
                                       getSourceSpan(*ctx->IDENTIFIER()));
    }

    std::any AntlrASTGeneratorVisitor::visitStatement(CeresParser::StatementContext *ctx) {
        NOT_IMPLEMENTED();
    }

    std::any AntlrASTGeneratorVisitor::visitReturnStatement(CeresParser::ReturnStatementContext *ctx) {
        NOT_IMPLEMENTED();
    }

    std::any AntlrASTGeneratorVisitor::visitIfStatement(CeresParser::IfStatementContext *ctx) {
        NOT_IMPLEMENTED();
    }

    std::any AntlrASTGeneratorVisitor::visitWhileStatement(CeresParser::WhileStatementContext *ctx) {
        NOT_IMPLEMENTED();
    }

    std::any AntlrASTGeneratorVisitor::visitForStatement(CeresParser::ForStatementContext *ctx) {
        NOT_IMPLEMENTED();
    }

    std::any AntlrASTGeneratorVisitor::visitAssignment_expr(CeresParser::Assignment_exprContext *ctx) {
        ASSERT(ctx != nullptr);

        antlr4::Token* binaryOpToken = ctx->binary_op;

        ASSERT(binaryOpToken != nullptr);
        ASSERT(ctx->IDENTIFIER() != nullptr);

        auto expr = std::any_cast<Expression*>(visit(ctx->assignmentExpression()));

        std::optional<AST::BinaryOp> binaryOp{};
        switch(binaryOpToken->getType()) {
            case CeresLexer::ASSIGN_OP:
                // No binaryOp
                binaryOp.reset();
                break;
            case CeresLexer::PLUS_ASSIGN_OP:
                binaryOp = AST::BinaryOp::Sum;
                break;
            case CeresLexer::MINUS_ASSIGN_OP:
                binaryOp = AST::BinaryOp::Subtraction;
                break;
            case CeresLexer::MULT_ASSIGN_OP:
                binaryOp = AST::BinaryOp::Mult;
                break;
            case CeresLexer::DIV_ASSIGN_OP:
                binaryOp = AST::BinaryOp::Div;
                break;
            case CeresLexer::BITWISE_AND_ASSIGN_OP:
                binaryOp = AST::BinaryOp::BitwiseAnd;
                break;
            case CeresLexer::BITWISE_OR_ASSIGN_OP:
                binaryOp = AST::BinaryOp::BitwiseOr;
                break;
            case CeresLexer::BITWISE_XOR_ASSIGN_OP:
                binaryOp = AST::BinaryOp::BitwiseXor;
                break;
            case CeresLexer::BITWISE_RIGHT_SHIFT_ASSIGN_OP:
                binaryOp = AST::BinaryOp::BitshiftRight;
                break;
            case CeresLexer::BITWISE_LEFT_SHIFT_ASSIGN_OP:
                binaryOp = AST::BinaryOp::BitshiftLeft;
                break;
            case CeresLexer::MOD_ASSIGN_OP:
                binaryOp = AST::BinaryOp::Modulo;
                break;
            default:
                NOT_IMPLEMENTED();
                break;
        }

        return static_cast<Expression*>(new AssignmentExpression(getSourceSpan(*ctx), binaryOp, ctx->IDENTIFIER()->getText(),
                                                                 std::unique_ptr<Expression>(expr), getSourceSpan(binaryOpToken)));
    }

    std::any AntlrASTGeneratorVisitor::visitPostfix_expr(CeresParser::Postfix_exprContext *ctx) {
        ASSERT(ctx != nullptr);
        auto expr = std::any_cast<Expression*>(visit(ctx->assignmentExpression()));

        PostfixOp op;
        ASSERT(ctx->postfix != nullptr);

        switch(ctx->postfix->getType()) {
            case CeresLexer::UNARY_PLUS_PLUS_OP:
                op = PostfixOp::PostfixIncrement;
                break;
            case CeresLexer::UNARY_MINUS_MINUS_OP:
                op = PostfixOp::PostfixDecrement;
                break;
            default:
                NOT_IMPLEMENTED();
        }

        return static_cast<Expression*>(new PostfixExpression(getSourceSpan(*ctx), op,
                                                               std::unique_ptr<Expression>(expr),
                                                               getSourceSpan(ctx->postfix)));
    }

    std::any AntlrASTGeneratorVisitor::visitPrefix_expr(CeresParser::Prefix_exprContext *ctx) {
        ASSERT(ctx != nullptr);

        auto expr = std::any_cast<Expression*>(visit(ctx->assignmentExpression()));

        PrefixOp op;
        ASSERT(ctx->prefix != nullptr);

        switch(ctx->prefix->getType()) {
            case CeresLexer::UNARY_PLUS_PLUS_OP:
                op = PrefixOp::PrefixIncrement;
                break;
            case CeresLexer::UNARY_MINUS_MINUS_OP:
                op = PrefixOp::PrefixDecrement;
                break;
            case CeresLexer::PLUS_OP:
                op = PrefixOp::UnaryPlus;
                break;
            case CeresLexer::MINUS_OP:
                op = PrefixOp::UnaryMinus;
                break;
            case CeresLexer::LOGICAL_NOT:
                op = PrefixOp::UnaryLogicalNot;
                break;
            case CeresLexer::BITWISE_NOT:
                op = PrefixOp::UnaryBitwiseNot;
                break;
            default:
                NOT_IMPLEMENTED();
                break;
        }

        return static_cast<Expression*>(new PrefixExpression(getSourceSpan(*ctx), op, std::unique_ptr<Expression>(expr), getSourceSpan(ctx->prefix)));
    }

    std::any AntlrASTGeneratorVisitor::visitFunction_call_expr(CeresParser::Function_call_exprContext *ctx) {
        ASSERT(ctx != nullptr);
        return visit(ctx->functionCall());
    }

    std::any AntlrASTGeneratorVisitor::visitPrimary_expr(CeresParser::Primary_exprContext *ctx) {
        ASSERT(ctx != nullptr);
        return visit(ctx->primaryExpression());
    }

    std::any AntlrASTGeneratorVisitor::visitBinary_op_expr(CeresParser::Binary_op_exprContext *ctx) {
        ASSERT(ctx != nullptr);

        BinaryOp op;
        SourceSpan opSpan = SourceSpan::createInvalidSpan();

        if(ctx->LOWER_OP().size() > 1) {
            // << (Left bitshift)
            ASSERT(ctx->LOWER_OP().size() == 2);
            op = BinaryOp::BitshiftLeft;
            opSpan = getSourceSpan(*ctx->LOWER_OP(0), *ctx->LOWER_OP(1));
        } else if(ctx->GREATER_OP().size() > 1) {
            // >> (Right bitshift)
            ASSERT(ctx->GREATER_OP().size() == 2);
            op = BinaryOp::BitshiftRight;
            opSpan = getSourceSpan(*ctx->GREATER_OP(0), *ctx->GREATER_OP(1));
        } else {
            ASSERT(ctx->binary_op != nullptr);
            opSpan = getSourceSpan(ctx->binary_op);
            switch(ctx->binary_op->getType()) {
                case CeresLexer::MULT_OP:
                    op = BinaryOp::Mult;
                    break;
                case CeresLexer::DIV_OP:
                    op = BinaryOp::Div;
                    break;
                case CeresLexer::MOD_OP:
                    op = BinaryOp::Modulo;
                    break;
                case CeresLexer::PLUS_OP:
                    op = BinaryOp::Sum;
                    break;
                case CeresLexer::MINUS_OP:
                    op = BinaryOp::Subtraction;
                    break;
                case CeresLexer::LOWER_OP:
                    op = BinaryOp::LessThan;
                    break;
                case CeresLexer::GREATER_OP:
                    op = BinaryOp::GreaterThan;
                    break;
                case CeresLexer::LOWER_EQUAL_OP:
                    op = BinaryOp::LessOrEqual;
                    break;
                case CeresLexer::GREATER_EQUAL_OP:
                    op = BinaryOp::GreaterOrEqual;
                    break;
                case CeresLexer::BITWISE_AND:
                    op = BinaryOp::BitwiseAnd;
                    break;
                case CeresLexer::BITWISE_XOR:
                    op = BinaryOp::BitwiseXor;
                    break;
                case CeresLexer::BITWISE_OR:
                    op = BinaryOp::BitwiseOr;
                    break;
                case CeresLexer::EQUAL_OP:
                    op = BinaryOp::Equals;
                    break;
                case CeresLexer::NOT_EQUAL_OP:
                    op = BinaryOp::NotEquals;
                    break;
                case CeresLexer::LOGICAL_AND_OP:
                    op = BinaryOp::LogicalAnd;
                    break;
                case CeresLexer::LOGICAL_OR_OP:
                    op = BinaryOp::LogicalOr;
                    break;
                default:
                    NOT_IMPLEMENTED();
                    break;
            }
        }

        auto left = std::any_cast<Expression*>(visit(ctx->assignmentExpression(0)));
        auto right = std::any_cast<Expression*>(visit(ctx->assignmentExpression(1)));

        return static_cast<Expression*>(new BinaryOperationExpression(getSourceSpan(*ctx), std::unique_ptr<Expression>(left),
                std::unique_ptr<Expression>(right), op, opSpan));
    }

    std::any AntlrASTGeneratorVisitor::visitExpression(CeresParser::ExpressionContext *ctx) {

        ASSERT(ctx != nullptr);

        // We must at least have one expression. Each expression is an expression separated by commas.
        size_t num_expressions = ctx->assignmentExpression().size();
        ASSERT(num_expressions >= 1);

        if (num_expressions == 1) {
            return visit(ctx->assignmentExpression()[0]);
        } else {
            // Create a comma expression node and return it
            std::vector<std::unique_ptr<Expression>> expressions;

            for(auto exprCtx : ctx->assignmentExpression()) {
                auto expr = std::any_cast<Expression*>(visit(exprCtx));
                ASSERT(expr != nullptr);

                expressions.push_back(std::unique_ptr<Expression>(expr));
            }

            return static_cast<Expression*>(new CommaExpression(getSourceSpan(*ctx), std::move(expressions)));
        }
    }

    std::any AntlrASTGeneratorVisitor::visitParens_expr(CeresParser::Parens_exprContext *ctx) {
        ASSERT(ctx != nullptr);
        return visit(ctx->expression());
    }

    std::any AntlrASTGeneratorVisitor::visitId_expr(CeresParser::Id_exprContext *ctx) {
        ASSERT(ctx != nullptr);
        ASSERT(ctx->IDENTIFIER() != nullptr);

        return static_cast<Expression*>(new IdentifierExpression(getSourceSpan(*ctx), ctx->IDENTIFIER()->getText()));
    }

    std::any AntlrASTGeneratorVisitor::visitInt_literal_expr(CeresParser::Int_literal_exprContext *ctx) {
        ASSERT(ctx != nullptr);
        return visit(ctx->intLiteral());
    }

    std::any AntlrASTGeneratorVisitor::visitFloat_literal_expr(CeresParser::Float_literal_exprContext *ctx) {
       ASSERT(ctx != nullptr);
       return visit(ctx->floatLiteral());
    }

    std::any AntlrASTGeneratorVisitor::visitBool_literal_expr(CeresParser::Bool_literal_exprContext *ctx) {
        ASSERT(ctx != nullptr);

        BoolLiteralValue value;

        ASSERT(ctx->BOOL_LITERAL() != nullptr);
        const std::string& text_literal = ctx->BOOL_LITERAL()->getText();

        if (text_literal == "true") {
            value = BoolLiteralValue::True;
        } else if (text_literal == "false") {
            value = BoolLiteralValue::False;
        } else {
            Log::panic("Bool literal has a value different that 'true' or 'false': {}", text_literal);
        }

        return static_cast<Expression*>( new BoolLiteral(getSourceSpan(*ctx), value) );
    }

    std::any AntlrASTGeneratorVisitor::visitIntLiteral(CeresParser::IntLiteralContext *ctx) {
        ASSERT(ctx != nullptr);

        IntLiteralType type = IntLiteralType::None;
        if (ctx->INTEGER_LITERAL_SUFFIX() != nullptr) {
            type = IntLiteralExpression::stringToIntLiteralType(ctx->INTEGER_LITERAL_SUFFIX()->getText());
            ASSERT(type != IntLiteralType::None);
        }

        IntLiteralBase base;

        std::string str;
        if (ctx->DEC_LITERAL() != nullptr) {
            base = IntLiteralBase::Dec;
            str = ctx->DEC_LITERAL()->getText();
        } else if (ctx->HEX_LITERAL() != nullptr) {
            base = IntLiteralBase::Hex;
            str = ctx->HEX_LITERAL()->getText().substr(2);
        } else if (ctx->OCT_LITERAL() != nullptr) {
            base = IntLiteralBase::Oct;
            str = ctx->HEX_LITERAL()->getText().substr(2);
        } else if (ctx->BIN_LITERAL() != nullptr) {
            base = IntLiteralBase::Bin;
            str = ctx->HEX_LITERAL()->getText().substr(2);
        } else {
            NOT_IMPLEMENTED();
        }

        return static_cast<Expression*>(new IntLiteralExpression(getSourceSpan(*ctx), base, type, str));
    }

    std::any AntlrASTGeneratorVisitor::visitFloatLiteral(CeresParser::FloatLiteralContext *ctx) {
        ASSERT(ctx != nullptr);

        FloatLiteralType type = FloatLiteralType::None;
        if (ctx->FLOAT_LITERAL_SUFFIX() != nullptr) {
            type = FloatLiteralExpression::stringToFloatLiteralType(ctx->FLOAT_LITERAL_SUFFIX()->getText());
            ASSERT(type != FloatLiteralType::None);
        }

        FloatLiteralBase base;

        std::string str;
        if(ctx->FLOAT_LITERAL() != nullptr) {
            str = ctx->FLOAT_LITERAL()->getText();
            base = FloatLiteralBase::Dec;
        } else if (ctx->DEC_LITERAL() != nullptr) {
            str = ctx->DEC_LITERAL()->getText();
            base = FloatLiteralBase::Dec;
        } else if (ctx->HEX_FLOAT_LITERAL() != nullptr) {
            str = ctx->HEX_FLOAT_LITERAL()->getText().substr(2);
            base = FloatLiteralBase::Hex;
        } else {
            NOT_IMPLEMENTED();
        }

        return static_cast<Expression*>(new FloatLiteralExpression(getSourceSpan(*ctx), base, type, str));
    }

    std::any AntlrASTGeneratorVisitor::visitFunctionCall(CeresParser::FunctionCallContext *ctx) {
        ASSERT(ctx != nullptr);
        ASSERT(ctx->IDENTIFIER() != nullptr);

        std::vector<std::unique_ptr<Expression>> args;
        args.reserve(ctx->assignmentExpression().size());

        for (auto assignmentExpressionContextPtr : ctx->assignmentExpression()) {
            auto res = visit(assignmentExpressionContextPtr);
            auto arg = std::any_cast<Expression*>(res);

            ASSERT(arg != nullptr);

            args.push_back(std::unique_ptr<Expression>(arg));
        }

        return static_cast<Expression*>(new FunctionCallExpression(getSourceSpan(*ctx),
                                                                   ctx->IDENTIFIER()->getText(), std::move(args),
                                                                   getSourceSpan(*ctx->IDENTIFIER())));
    }

}