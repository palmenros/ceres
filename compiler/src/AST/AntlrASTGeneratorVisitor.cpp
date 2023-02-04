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

#include "AntlrASTGeneratorVisitor.h"
#include "../utils/log.hpp"
#include "FunctionDefinition.h"
#include "VariableDeclaration.h"
#include "CompilationUnit.h"
#include "Type.h"

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

        return {startToken->getLine(), endToken->getLine(), startToken->getStartIndex(), endToken->getStopIndex()};
    }

    SourceSpan AntlrASTGeneratorVisitor::getSourceSpan(const antlr4::tree::TerminalNode& context) {
        auto tok = context.getSymbol();

        ASSERT(tok != nullptr);

        return {tok->getLine(), tok->getLine(), tok->getStartIndex(), tok->getStopIndex()};
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

        return Type{ctx->getText()};
    }

    std::any AntlrASTGeneratorVisitor::visitVarDeclaration(CeresParser::VarDeclarationContext *ctx) {
        ASSERT(ctx != nullptr);

        ASSERT(ctx->IDENTIFIER() != nullptr);
        ASSERT(ctx->type() != nullptr);

        std::unique_ptr<Expression> initializer_expression = nullptr;
        if(ctx->expression() != nullptr) {
            auto expr = std::any_cast<Expression*>(visit(ctx->expression()));
            initializer_expression.reset(expr);
        }

        VariableConstness constness = VariableConstness::Const;
        if(ctx->VAR() != nullptr) {
            constness = VariableConstness::NonConst;
        } else {
            ASSERT(ctx->CONSTANT() != nullptr);
        }

        auto type = std::any_cast<Type>(visit(ctx->type()));
        return new VariableDeclaration(getSourceSpan(*ctx), std::move(initializer_expression), VariableVisibility::Private,
                                       constness, type, ctx->IDENTIFIER()->getText(), getSourceSpan(*ctx->type()), getSourceSpan(*ctx->IDENTIFIER()));
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
        NOT_IMPLEMENTED();
    }

    std::any AntlrASTGeneratorVisitor::visitPostfix_expr(CeresParser::Postfix_exprContext *ctx) {
        NOT_IMPLEMENTED();
    }

    std::any AntlrASTGeneratorVisitor::visitPrefix_expr(CeresParser::Prefix_exprContext *ctx) {
        NOT_IMPLEMENTED();
    }

    std::any AntlrASTGeneratorVisitor::visitFunction_call_expr(CeresParser::Function_call_exprContext *ctx) {
        NOT_IMPLEMENTED();
    }

    std::any AntlrASTGeneratorVisitor::visitPrimary_expr(CeresParser::Primary_exprContext *ctx) {
        NOT_IMPLEMENTED();
    }

    std::any AntlrASTGeneratorVisitor::visitBinary_op_expr(CeresParser::Binary_op_exprContext *ctx) {
        NOT_IMPLEMENTED();
    }

    std::any AntlrASTGeneratorVisitor::visitExpression(CeresParser::ExpressionContext *ctx) {
        NOT_IMPLEMENTED();
    }

    std::any AntlrASTGeneratorVisitor::visitParens_expr(CeresParser::Parens_exprContext *ctx) {
        NOT_IMPLEMENTED();
    }

    std::any AntlrASTGeneratorVisitor::visitId_expr(CeresParser::Id_exprContext *ctx) {
        NOT_IMPLEMENTED();
    }

    std::any AntlrASTGeneratorVisitor::visitInt_literal_expr(CeresParser::Int_literal_exprContext *ctx) {
        NOT_IMPLEMENTED();
    }

    std::any AntlrASTGeneratorVisitor::visitFloat_literal_expr(CeresParser::Float_literal_exprContext *ctx) {
        NOT_IMPLEMENTED();
    }

    std::any AntlrASTGeneratorVisitor::visitBool_literal_expr(CeresParser::Bool_literal_exprContext *ctx) {
        NOT_IMPLEMENTED();
    }

    std::any AntlrASTGeneratorVisitor::visitIntLiteral(CeresParser::IntLiteralContext *ctx) {
        NOT_IMPLEMENTED();
    }

    std::any AntlrASTGeneratorVisitor::visitFloatLiteral(CeresParser::FloatLiteralContext *ctx) {
        NOT_IMPLEMENTED();
    }

    std::any AntlrASTGeneratorVisitor::visitFunctionCall(CeresParser::FunctionCallContext *ctx) {
        NOT_IMPLEMENTED();
    }

}