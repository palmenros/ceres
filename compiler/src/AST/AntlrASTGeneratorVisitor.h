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

#ifndef COMPILER_ANTLRASTGENERATORVISITOR_H
#define COMPILER_ANTLRASTGENERATORVISITOR_H

#include "CeresParserBaseVisitor.h"
#include "SourceSpan.h"

namespace Ceres::AST {

    class AntlrASTGeneratorVisitor : public antlrgenerated::CeresParserVisitor {

    private:

        static SourceSpan getSourceSpan(const antlr4::ParserRuleContext& context);

        static SourceSpan getSourceSpan(const antlr4::tree::TerminalNode& context);

    public:

        std::any visitErrorNode(antlr4::tree::ErrorNode *node) override;

        std::any visitCompilationUnit(antlrgenerated::CeresParser::CompilationUnitContext *ctx) override;

        std::any visitGlobalVarDeclaration(antlrgenerated::CeresParser::GlobalVarDeclarationContext *ctx) override;

        std::any visitFunctionDefinition(antlrgenerated::CeresParser::FunctionDefinitionContext *ctx) override;

        std::any visitFormalParameters(antlrgenerated::CeresParser::FormalParametersContext *ctx) override;

        std::any visitParameter(antlrgenerated::CeresParser::ParameterContext *ctx) override;

        std::any visitBlock(antlrgenerated::CeresParser::BlockContext *ctx) override;

        std::any visitType(antlrgenerated::CeresParser::TypeContext *ctx) override;

        std::any visitVarDeclaration(antlrgenerated::CeresParser::VarDeclarationContext *ctx) override;

        std::any visitStatement(antlrgenerated::CeresParser::StatementContext *ctx) override;

        std::any visitReturnStatement(antlrgenerated::CeresParser::ReturnStatementContext *ctx) override;

        std::any visitIfStatement(antlrgenerated::CeresParser::IfStatementContext *ctx) override;

        std::any visitWhileStatement(antlrgenerated::CeresParser::WhileStatementContext *ctx) override;

        std::any visitForStatement(antlrgenerated::CeresParser::ForStatementContext *ctx) override;

        std::any visitAssignment_expr(antlrgenerated::CeresParser::Assignment_exprContext *ctx) override;

        std::any visitPostfix_expr(antlrgenerated::CeresParser::Postfix_exprContext *ctx) override;

        std::any visitPrefix_expr(antlrgenerated::CeresParser::Prefix_exprContext *ctx) override;

        std::any visitFunction_call_expr(antlrgenerated::CeresParser::Function_call_exprContext *ctx) override;

        std::any visitPrimary_expr(antlrgenerated::CeresParser::Primary_exprContext *ctx) override;

        std::any visitBinary_op_expr(antlrgenerated::CeresParser::Binary_op_exprContext *ctx) override;

        std::any visitExpression(antlrgenerated::CeresParser::ExpressionContext *ctx) override;

        std::any visitParens_expr(antlrgenerated::CeresParser::Parens_exprContext *ctx) override;

        std::any visitId_expr(antlrgenerated::CeresParser::Id_exprContext *ctx) override;

        std::any visitInt_literal_expr(antlrgenerated::CeresParser::Int_literal_exprContext *ctx) override;

        std::any visitFloat_literal_expr(antlrgenerated::CeresParser::Float_literal_exprContext *ctx) override;

        std::any visitBool_literal_expr(antlrgenerated::CeresParser::Bool_literal_exprContext *ctx) override;

        std::any visitIntLiteral(antlrgenerated::CeresParser::IntLiteralContext *ctx) override;

        std::any visitFloatLiteral(antlrgenerated::CeresParser::FloatLiteralContext *ctx) override;

        std::any visitFunctionCall(antlrgenerated::CeresParser::FunctionCallContext *ctx) override;

        static SourceSpan getSourceSpan(const antlr4::Token *token);

        static SourceSpan getSourceSpan(const antlr4::Token *first, const antlr4::Token *second);

        static SourceSpan getSourceSpan(const antlr4::tree::TerminalNode &start, const antlr4::tree::TerminalNode &end);
    };

} // Ceres

#endif //COMPILER_ANTLRASTGENERATORVISITOR_H
