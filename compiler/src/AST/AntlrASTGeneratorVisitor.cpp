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
#include "../Diagnostics/Diagnostics.h"
#include "../Typing/Type.h"
#include "../utils/log.hpp"
#include "CeresLexer.h"
#include "CeresParser.h"
#include "nodes/AssignmentExpression.h"
#include "nodes/BinaryOperationExpression.h"
#include "nodes/BlockStatement.h"
#include "nodes/BoolLiteral.h"
#include "nodes/CommaExpression.h"
#include "nodes/CompilationUnit.h"
#include "nodes/ExpressionStatement.h"
#include "nodes/FloatLiteralExpression.h"
#include "nodes/ForStatement.h"
#include "nodes/FunctionCallExpression.h"
#include "nodes/FunctionDefinition.h"
#include "nodes/IdentifierExpression.h"
#include "nodes/IfStatement.h"
#include "nodes/IntLiteralExpression.h"
#include "nodes/PostfixExpression.h"
#include "nodes/PrefixExpression.h"
#include "nodes/ReturnStatement.h"
#include "nodes/VariableDeclaration.h"
#include "nodes/VariableDeclarationStatement.h"
#include "nodes/WhileStatement.h"
#include <memory>
#include <optional>

using namespace antlrgenerated;

namespace Ceres::AST {

    // This class is used for synchronizing the AST
    class ParseException : public std::runtime_error {
    public:
        ParseException()
            : std::runtime_error("ParseException. Should not escape AntlrASTGeneratorVisitor!") {}
    };

    AntlrASTGeneratorVisitor::AntlrASTGeneratorVisitor(unsigned int fileId) : fileId(fileId) {}

    void AntlrASTGeneratorVisitor::handleParseError() { throw ParseException(); }

    std::any AntlrASTGeneratorVisitor::visitErrorNode(antlr4::tree::ErrorNode *node) {
        handleParseError();
    }

    std::any AntlrASTGeneratorVisitor::defaultResult() {
        // If we ever get here, we have reached an Error in a labeled rule, so no visit will be called
        // We should throw an exception so we can catch it where relevant in the AST.
        handleParseError();
    }

    std::any AntlrASTGeneratorVisitor::visit(antlr4::tree::ParseTree *tree) {
        if (tree == nullptr) {
            // There has been a parse error here
            handleParseError();
        }

        return tree->accept(this);
    }

    std::any AntlrASTGeneratorVisitor::visitTerminal(antlr4::tree::TerminalNode *node) {
        ASSERT(node != nullptr);
        return node->getText();
    }


    void AntlrASTGeneratorVisitor::checkException(const antlr4::ParserRuleContext &context) {
        if (context.exception != nullptr) { handleParseError(); }
    }

    SourceSpan AntlrASTGeneratorVisitor::getSourceSpan(const antlr4::ParserRuleContext &context) {
        auto startToken = context.getStart();
        auto endToken = context.getStop();

        ASSERT(startToken != nullptr);
        ASSERT(endToken != nullptr);

        return getSourceSpan(startToken, endToken);
    }

    SourceSpan AntlrASTGeneratorVisitor::getSourceSpan(const antlr4::tree::TerminalNode &context) {
        auto tok = context.getSymbol();

        ASSERT(tok != nullptr);

        return getSourceSpan(tok);
    }

    SourceSpan AntlrASTGeneratorVisitor::getSourceSpan(const antlr4::tree::TerminalNode &start,
                                                       const antlr4::tree::TerminalNode &end) {
        auto startTok = start.getSymbol();
        auto endTok = end.getSymbol();

        ASSERT(startTok != nullptr);
        ASSERT(endTok != nullptr);

        return getSourceSpan(startTok, endTok);
    }

    SourceSpan AntlrASTGeneratorVisitor::getSourceSpan(const antlr4::Token *tok) const {
        return {fileId, tok->getStartIndex(), tok->getStopIndex()};
    }

    SourceSpan AntlrASTGeneratorVisitor::getSourceSpan(const antlr4::Token *startToken,
                                                       const antlr4::Token *endToken) const {
        return {fileId, startToken->getStartIndex(), endToken->getStopIndex()};
    }

    std::any
    AntlrASTGeneratorVisitor::visitCompilationUnit(CeresParser::CompilationUnitContext *ctx) {
        ASSERT(ctx != nullptr);
        if (ctx->exception != nullptr) { return new CompilationUnit(getSourceSpan(*ctx)); }

        std::vector<std::unique_ptr<FunctionDefinition>> functionDefinitions;
        std::vector<std::unique_ptr<VariableDeclaration>> variableDeclarations;

        functionDefinitions.reserve(ctx->functionDefinition().size());

        for (auto funDefinitionContextPtr: ctx->functionDefinition()) {
            try {
                auto res = visit(funDefinitionContextPtr);
                auto funDef = std::unique_ptr<FunctionDefinition>(
                        std::any_cast<FunctionDefinition *>(res));

                ASSERT(funDef != nullptr);

                functionDefinitions.push_back(std::move(funDef));
            } catch (ParseException &) {
                //Log::debug("Parse exception in function definition caught");
            }
        }

        variableDeclarations.reserve(ctx->globalVarDeclaration().size());

        for (auto varDefinitionContextPtr: ctx->globalVarDeclaration()) {
            try {
                auto res = visit(varDefinitionContextPtr);
                auto varDef = std::unique_ptr<VariableDeclaration>(
                        std::any_cast<VariableDeclaration *>(res));

                ASSERT(varDef != nullptr);

                variableDeclarations.push_back(std::move(varDef));
            } catch (ParseException &) {
                //Log::debug("Parse exception in variable definition caught");
            }
        }

        return new CompilationUnit(getSourceSpan(*ctx), std::move(functionDefinitions),
                                   std::move(variableDeclarations));
    }

    std::any AntlrASTGeneratorVisitor::visitGlobalVarDeclaration(
            CeresParser::GlobalVarDeclarationContext *ctx) {
        ASSERT(ctx != nullptr);
        checkException(*ctx);

        auto varDeclaration = std::any_cast<VariableDeclaration *>(visit(ctx->varDeclaration()));
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

    std::any
    AntlrASTGeneratorVisitor::visitFunctionDefinition(CeresParser::FunctionDefinitionContext *ctx) {
        ASSERT(ctx != nullptr);
        checkException(*ctx);

        FunctionVisibility visibility = FunctionVisibility::Private;

        if (ctx->PUB() != nullptr) { visibility = FunctionVisibility::Public; }

        ASSERT(ctx->IDENTIFIER() != nullptr);
        SourceSpan identifierSourceSpan = getSourceSpan(*ctx->IDENTIFIER());
        SourceSpan returnTypeSourceSpan = SourceSpan::createInvalidSpan();

        auto function_name = std::any_cast<std::string>(visit(ctx->IDENTIFIER()));
        Type *returnType;

        if (ctx->type() != nullptr) {
            returnType = std::any_cast<Type *>(visit(ctx->type()));
            returnTypeSourceSpan = getSourceSpan(*ctx->type());
        } else {
            returnType = UnitVoidType::get();
        }

        std::vector<FunctionParameter> parameters;

        if (ctx->formalParameters() != nullptr) {
            parameters =
                    std::any_cast<std::vector<FunctionParameter>>(visit(ctx->formalParameters()));
        }

        auto statement = std::any_cast<Statement *>(visit(ctx->block()));

        // TODO: Remove all dynamic_cast by using an LLVM-like RTTI. Maybe check casts only in debug, as they
        //          should always be correct.
        auto block = std::unique_ptr<BlockStatement>(dynamic_cast<BlockStatement *>(statement));

        return new FunctionDefinition(getSourceSpan(*ctx), visibility, function_name,
                                      std::move(parameters), returnType, std::move(block),
                                      returnTypeSourceSpan, identifierSourceSpan);
    }

    std::any
    AntlrASTGeneratorVisitor::visitFormalParameters(CeresParser::FormalParametersContext *ctx) {
        ASSERT(ctx != nullptr);
        checkException(*ctx);

        std::vector<FunctionParameter> parameters;
        parameters.reserve(ctx->parameter().size());

        for (auto parameterContext: ctx->parameter()) {
            try {
                auto res = std::any_cast<FunctionParameter>(visit(parameterContext));
                parameters.push_back(res);
            } catch (ParseException &) {
                //Log::debug("Caught Parse exception in parameter");
            }
        }

        return parameters;
    }

    std::any AntlrASTGeneratorVisitor::visitParameter(CeresParser::ParameterContext *ctx) {
        ASSERT(ctx != nullptr);
        checkException(*ctx);

        VariableConstness constness = VariableConstness::Const;

        ASSERT(ctx->IDENTIFIER() != nullptr);
        ASSERT(ctx->type() != nullptr);

        Type *type = std::any_cast<Type *>(visit(ctx->type()));

        if (ctx->VAR() != nullptr) { constness = VariableConstness::NonConst; }

        SourceSpan typeSourceSpan = getSourceSpan(*ctx->type());
        SourceSpan nameSourceSpan = getSourceSpan(*ctx->IDENTIFIER());

        auto parameterName = std::any_cast<std::string>(visit(ctx->IDENTIFIER()));
        return FunctionParameter{type, parameterName, constness, typeSourceSpan, nameSourceSpan};
    }

    std::any AntlrASTGeneratorVisitor::visitBlock(CeresParser::BlockContext *ctx) {
        ASSERT(ctx != nullptr);
        checkException(*ctx);

        std::vector<std::unique_ptr<Statement>> statements;

        for (auto statementContextPtr: ctx->statement()) {
            try {
                auto res = std::unique_ptr<Statement>(
                        std::any_cast<Statement *>(visit(statementContextPtr)));
                if (res != nullptr) {
                    // Note: the statement can be a nullptr, for empty statements such as ';;'
                    statements.push_back(std::move(res));
                }
            } catch (ParseException &) {
                //Log::debug("Parse exception in block caught");
            }
        }

        return static_cast<Statement *>(
                new BlockStatement(getSourceSpan(*ctx), std::move(statements)));
    }

    std::any AntlrASTGeneratorVisitor::visitType(CeresParser::TypeContext *ctx) {
        ASSERT(ctx != nullptr);
        checkException(*ctx);

        if (ctx->IDENTIFIER() != nullptr) {
            return static_cast<Type *>(UnresolvedType::get(ctx->IDENTIFIER()->toString()));
        } else if (ctx->INTEGER_LITERAL_SUFFIX() != nullptr) {
            return static_cast<Type *>(
                    PrimitiveScalarType::get(ctx->INTEGER_LITERAL_SUFFIX()->toString()));
        } else if (ctx->FLOAT_LITERAL_SUFFIX() != nullptr) {
            return static_cast<Type *>(
                    PrimitiveScalarType::get(ctx->FLOAT_LITERAL_SUFFIX()->toString()));
        } else {
            NOT_IMPLEMENTED();
        }
    }

    std::any
    AntlrASTGeneratorVisitor::visitVarDeclaration(CeresParser::VarDeclarationContext *ctx) {
        ASSERT(ctx != nullptr);
        checkException(*ctx);

        ASSERT(ctx->IDENTIFIER() != nullptr);

        std::unique_ptr<Expression> initializer_expression = nullptr;
        if (ctx->expression() != nullptr) {
            initializer_expression.reset(std::any_cast<Expression *>(visit(ctx->expression())));
        }

        VariableConstness constness;
        if (ctx->VAR() != nullptr) {
            constness = VariableConstness::NonConst;
        } else if (ctx->CONSTANT() != nullptr) {
            constness = VariableConstness::Const;
        } else {
            NOT_IMPLEMENTED();
        }

        Type *type;
        SourceSpan typeSourceSpan = SourceSpan::createInvalidSpan();
        if (ctx->type() != nullptr) {
            type = std::any_cast<Type *>(visit(ctx->type()));
            typeSourceSpan = getSourceSpan(*ctx->type());
        } else {
            type = NotYetInferredType::get(NotYetInferredKind::VariableDeclaration);
        }

        auto var_name = std::any_cast<std::string>(visit(ctx->IDENTIFIER()));

        return new VariableDeclaration(getSourceSpan(*ctx), std::move(initializer_expression),
                                       VariableVisibility::Private, constness, VariableScope::Local,
                                       type, var_name, typeSourceSpan,
                                       getSourceSpan(*ctx->IDENTIFIER()));
    }

    std::any
    AntlrASTGeneratorVisitor::visitReturnStatement(CeresParser::ReturnStatementContext *ctx) {
        ASSERT(ctx != nullptr);
        checkException(*ctx);

        auto expr = std::any_cast<Expression *>(visit(ctx->expression()));
        return static_cast<Statement *>(
                new ReturnStatement(getSourceSpan(*ctx), std::unique_ptr<Expression>(expr)));
    }

    std::any AntlrASTGeneratorVisitor::visitIfStatement(CeresParser::IfStatementContext *ctx) {
        ASSERT(ctx != nullptr);
        checkException(*ctx);

        auto condition =
                std::unique_ptr<Expression>(std::any_cast<Expression *>(visit(ctx->expression())));
        auto thenBlock = std::unique_ptr<BlockStatement>(
                dynamic_cast<BlockStatement *>(std::any_cast<Statement *>(visit(ctx->block(0)))));

        std::unique_ptr<Statement> elseStatement = nullptr;
        if (ctx->else_block != nullptr) {
            elseStatement.reset(std::any_cast<Statement *>(visit(ctx->else_block)));
        } else if (ctx->else_if != nullptr) {
            elseStatement.reset(std::any_cast<Statement *>(visit(ctx->else_if)));
        }

        ASSERT(condition != nullptr);
        ASSERT(thenBlock != nullptr);

        return static_cast<Statement *>(new IfStatement(getSourceSpan(*ctx), std::move(condition),
                                                        std::move(thenBlock),
                                                        std::move(elseStatement)));
    }

    std::any
    AntlrASTGeneratorVisitor::visitWhileStatement(CeresParser::WhileStatementContext *ctx) {
        ASSERT(ctx != nullptr);
        checkException(*ctx);

        auto condition =
                std::unique_ptr<Expression>(std::any_cast<Expression *>(visit(ctx->expression())));
        auto body = std::unique_ptr<BlockStatement>(
                dynamic_cast<BlockStatement *>(std::any_cast<Statement *>(visit(ctx->block()))));

        ASSERT(condition != nullptr);
        ASSERT(body != nullptr);

        return static_cast<Statement *>(
                new WhileStatement(getSourceSpan(*ctx), std::move(condition), std::move(body)));
    }

    std::any AntlrASTGeneratorVisitor::visitForStatement(CeresParser::ForStatementContext *ctx) {
        ASSERT(ctx != nullptr);
        checkException(*ctx);

        std::unique_ptr<VariableDeclaration> varDecl = nullptr;
        std::unique_ptr<Expression> declExpr = nullptr;
        std::unique_ptr<Expression> condExpr = nullptr;
        std::unique_ptr<Expression> updateExpr = nullptr;

        auto body = std::unique_ptr<BlockStatement>(
                dynamic_cast<BlockStatement *>(std::any_cast<Statement *>(visit(ctx->block()))));

        if (ctx->varDeclaration() != nullptr) {
            varDecl.reset(std::any_cast<VariableDeclaration *>(visit(ctx->varDeclaration())));
        } else if (ctx->decl_expr != nullptr) {
            declExpr.reset(std::any_cast<Expression *>(visit(ctx->decl_expr)));
        }

        if (ctx->cond_expr != nullptr) {
            condExpr.reset(std::any_cast<Expression *>(visit(ctx->cond_expr)));
        }

        if (ctx->update_expr != nullptr) {
            updateExpr.reset(std::any_cast<Expression *>(visit(ctx->update_expr)));
        }

        ASSERT(body != nullptr);

        return static_cast<Statement *>(new ForStatement(getSourceSpan(*ctx), std::move(varDecl),
                                                         std::move(declExpr), std::move(condExpr),
                                                         std::move(updateExpr), std::move(body)));
    }

    std::any
    AntlrASTGeneratorVisitor::visitAssignment_expr(CeresParser::Assignment_exprContext *ctx) {
        ASSERT(ctx != nullptr);
        checkException(*ctx);

        antlr4::Token *binaryOpToken = ctx->binary_op;

        ASSERT(binaryOpToken != nullptr);
        ASSERT(ctx->IDENTIFIER() != nullptr);

        auto expr = std::unique_ptr<Expression>(
                std::any_cast<Expression *>(visit(ctx->assignmentExpression())));

        std::optional<AST::BinaryOp> binaryOp{};
        switch (binaryOpToken->getType()) {
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

        auto LHS_identifier = std::any_cast<std::string>(visit(ctx->IDENTIFIER()));

        return static_cast<Expression *>(new AssignmentExpression(getSourceSpan(*ctx), binaryOp,
                                                                  LHS_identifier, std::move(expr),
                                                                  getSourceSpan(binaryOpToken)));
    }

    std::any AntlrASTGeneratorVisitor::visitPostfix_expr(CeresParser::Postfix_exprContext *ctx) {
        ASSERT(ctx != nullptr);
        checkException(*ctx);

        auto expr = std::unique_ptr<Expression>(
                std::any_cast<Expression *>(visit(ctx->assignmentExpression())));

        PostfixOp op;
        ASSERT(ctx->postfix != nullptr);

        switch (ctx->postfix->getType()) {
            case CeresLexer::UNARY_PLUS_PLUS_OP:
                op = PostfixOp::PostfixIncrement;
                break;
            case CeresLexer::UNARY_MINUS_MINUS_OP:
                op = PostfixOp::PostfixDecrement;
                break;
            default:
                NOT_IMPLEMENTED();
        }

        return static_cast<Expression *>(new PostfixExpression(
                getSourceSpan(*ctx), op, std::move(expr), getSourceSpan(ctx->postfix)));
    }

    std::any AntlrASTGeneratorVisitor::visitPrefix_expr(CeresParser::Prefix_exprContext *ctx) {
        ASSERT(ctx != nullptr);
        checkException(*ctx);

        auto expr = std::unique_ptr<Expression>(
                std::any_cast<Expression *>(visit(ctx->assignmentExpression())));

        PrefixOp op;
        ASSERT(ctx->prefix != nullptr);

        switch (ctx->prefix->getType()) {
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

        return static_cast<Expression *>(new PrefixExpression(
                getSourceSpan(*ctx), op, std::move(expr), getSourceSpan(ctx->prefix)));
    }

    std::any
    AntlrASTGeneratorVisitor::visitFunction_call_expr(CeresParser::Function_call_exprContext *ctx) {
        ASSERT(ctx != nullptr);
        checkException(*ctx);

        return visit(ctx->functionCall());
    }

    std::any AntlrASTGeneratorVisitor::visitPrimary_expr(CeresParser::Primary_exprContext *ctx) {
        ASSERT(ctx != nullptr);
        checkException(*ctx);

        return visit(ctx->primaryExpression());
    }

    std::any
    AntlrASTGeneratorVisitor::visitBinary_op_expr(CeresParser::Binary_op_exprContext *ctx) {
        ASSERT(ctx != nullptr);
        checkException(*ctx);

        BinaryOp op;
        SourceSpan opSpan = SourceSpan::createInvalidSpan();

        if (ctx->LOWER_OP().size() > 1) {
            // << (Left bitshift)
            ASSERT(ctx->LOWER_OP().size() == 2);
            op = BinaryOp::BitshiftLeft;
            opSpan = getSourceSpan(*ctx->LOWER_OP(0), *ctx->LOWER_OP(1));
        } else if (ctx->GREATER_OP().size() > 1) {
            // >> (Right bitshift)
            ASSERT(ctx->GREATER_OP().size() == 2);
            op = BinaryOp::BitshiftRight;
            opSpan = getSourceSpan(*ctx->GREATER_OP(0), *ctx->GREATER_OP(1));
        } else {
            ASSERT(ctx->binary_op != nullptr);
            opSpan = getSourceSpan(ctx->binary_op);
            switch (ctx->binary_op->getType()) {
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

        auto left = std::unique_ptr<Expression>(
                std::any_cast<Expression *>(visit(ctx->assignmentExpression(0))));
        auto right = std::unique_ptr<Expression>(
                std::any_cast<Expression *>(visit(ctx->assignmentExpression(1))));

        return static_cast<Expression *>(new BinaryOperationExpression(
                getSourceSpan(*ctx), std::move(left), std::move(right), op, opSpan));
    }

    std::any AntlrASTGeneratorVisitor::visitExpression(CeresParser::ExpressionContext *ctx) {
        ASSERT(ctx != nullptr);
        checkException(*ctx);

        // We must at least have one expression. Each expression is an expression separated by commas.
        size_t num_expressions = ctx->assignmentExpression().size();
        ASSERT(num_expressions >= 1);

        if (num_expressions == 1) {
            return visit(ctx->assignmentExpression()[0]);
        } else {
            // Create a comma expression node and return it
            std::vector<std::unique_ptr<Expression>> expressions;

            for (auto exprCtx: ctx->assignmentExpression()) {
                auto expr =
                        std::unique_ptr<Expression>(std::any_cast<Expression *>(visit(exprCtx)));
                ASSERT(expr != nullptr);

                expressions.push_back(std::move(expr));
            }

            return static_cast<Expression *>(
                    new CommaExpression(getSourceSpan(*ctx), std::move(expressions)));
        }
    }

    std::any AntlrASTGeneratorVisitor::visitParens_expr(CeresParser::Parens_exprContext *ctx) {
        ASSERT(ctx != nullptr);
        checkException(*ctx);

        return visit(ctx->expression());
    }

    std::any AntlrASTGeneratorVisitor::visitId_expr(CeresParser::Id_exprContext *ctx) {
        ASSERT(ctx != nullptr);
        checkException(*ctx);

        ASSERT(ctx->IDENTIFIER() != nullptr);

        auto id = std::any_cast<std::string>(visit(ctx->IDENTIFIER()));

        return static_cast<Expression *>(new IdentifierExpression(getSourceSpan(*ctx), id));
    }

    std::any
    AntlrASTGeneratorVisitor::visitInt_literal_expr(CeresParser::Int_literal_exprContext *ctx) {
        ASSERT(ctx != nullptr);
        checkException(*ctx);

        return visit(ctx->intLiteral());
    }

    std::any
    AntlrASTGeneratorVisitor::visitFloat_literal_expr(CeresParser::Float_literal_exprContext *ctx) {
        ASSERT(ctx != nullptr);
        checkException(*ctx);

        return visit(ctx->floatLiteral());
    }

    std::any
    AntlrASTGeneratorVisitor::visitBool_literal_expr(CeresParser::Bool_literal_exprContext *ctx) {
        ASSERT(ctx != nullptr);
        checkException(*ctx);

        BoolLiteralValue value;

        ASSERT(ctx->BOOL_LITERAL() != nullptr);
        const std::string &text_literal = std::any_cast<std::string>(visit(ctx->BOOL_LITERAL()));

        if (text_literal == "true") {
            value = BoolLiteralValue::True;
        } else if (text_literal == "false") {
            value = BoolLiteralValue::False;
        } else {
            Log::panic("Bool literal has a value different that 'true' or 'false': {}",
                       text_literal);
        }

        return static_cast<Expression *>(new BoolLiteral(getSourceSpan(*ctx), value));
    }

    std::any AntlrASTGeneratorVisitor::visitIntLiteral(CeresParser::IntLiteralContext *ctx) {
        ASSERT(ctx != nullptr);
        checkException(*ctx);

        Type *type;
        if (ctx->INTEGER_LITERAL_SUFFIX() != nullptr) {
            auto suffix = std::any_cast<std::string>(visit(ctx->INTEGER_LITERAL_SUFFIX()));
            type = PrimitiveScalarType::get(suffix);
        } else {
            type = NotYetInferredType::get(NotYetInferredKind::NumberLiteral);
        }

        IntLiteralBase base;

        std::string str;
        if (ctx->DEC_LITERAL() != nullptr) {
            base = IntLiteralBase::Dec;
            str = std::any_cast<std::string>(visit(ctx->DEC_LITERAL()));
        } else if (ctx->HEX_LITERAL() != nullptr) {
            base = IntLiteralBase::Hex;
            str = std::any_cast<std::string>(visit(ctx->HEX_LITERAL())).substr(2);
        } else if (ctx->OCT_LITERAL() != nullptr) {
            base = IntLiteralBase::Oct;
            str = std::any_cast<std::string>(visit(ctx->OCT_LITERAL())).substr(2);
        } else if (ctx->BIN_LITERAL() != nullptr) {
            base = IntLiteralBase::Bin;
            str = std::any_cast<std::string>(visit(ctx->BIN_LITERAL())).substr(2);
        } else {
            NOT_IMPLEMENTED();
        }

        return static_cast<Expression *>(
                new IntLiteralExpression(getSourceSpan(*ctx), base, type, str));
    }

    std::any AntlrASTGeneratorVisitor::visitFloatLiteral(CeresParser::FloatLiteralContext *ctx) {
        ASSERT(ctx != nullptr);
        checkException(*ctx);

        Type *type;
        if (ctx->FLOAT_LITERAL_SUFFIX() != nullptr) {
            auto suffix = std::any_cast<std::string>(visit(ctx->FLOAT_LITERAL_SUFFIX()));
            type = PrimitiveScalarType::get(suffix);
        } else {
            type = NotYetInferredType::get(NotYetInferredKind::NumberLiteral);
        }

        FloatLiteralBase base;

        std::string str;
        if (ctx->FLOAT_LITERAL() != nullptr) {
            str = std::any_cast<std::string>(visit(ctx->FLOAT_LITERAL()));
            base = FloatLiteralBase::Dec;
        } else if (ctx->DEC_LITERAL() != nullptr) {
            str = std::any_cast<std::string>(visit(ctx->DEC_LITERAL()));
            base = FloatLiteralBase::Dec;
        } else if (ctx->HEX_FLOAT_LITERAL() != nullptr) {
            str = std::any_cast<std::string>(visit(ctx->HEX_FLOAT_LITERAL())).substr(2);
            base = FloatLiteralBase::Hex;
        } else {
            NOT_IMPLEMENTED();
        }

        return static_cast<Expression *>(
                new FloatLiteralExpression(getSourceSpan(*ctx), base, type, str));
    }

    std::any AntlrASTGeneratorVisitor::visitFunctionCall(CeresParser::FunctionCallContext *ctx) {
        ASSERT(ctx != nullptr);
        checkException(*ctx);

        ASSERT(ctx->IDENTIFIER() != nullptr);

        std::vector<std::unique_ptr<Expression>> args;
        args.reserve(ctx->assignmentExpression().size());

        for (auto assignmentExpressionContextPtr: ctx->assignmentExpression()) {
            auto res = visit(assignmentExpressionContextPtr);
            auto arg = std::unique_ptr<Expression>(std::any_cast<Expression *>(res));

            ASSERT(arg != nullptr);

            args.push_back(std::move(arg));
        }

        auto function_identifier = std::any_cast<std::string>(visit(ctx->IDENTIFIER()));

        return static_cast<Expression *>(
                new FunctionCallExpression(getSourceSpan(*ctx), function_identifier,
                                           std::move(args), getSourceSpan(*ctx->IDENTIFIER())));
    }

    std::any
    AntlrASTGeneratorVisitor::visitVar_decl_statement(CeresParser::Var_decl_statementContext *ctx) {
        ASSERT(ctx != nullptr);
        checkException(*ctx);

        auto varDeclaration = std::unique_ptr<VariableDeclaration>(
                std::any_cast<VariableDeclaration *>(visit(ctx->varDeclaration())));

        return static_cast<Statement *>(
                new VariableDeclarationStatement(getSourceSpan(*ctx), std::move(varDeclaration)));
    }

    std::any
    AntlrASTGeneratorVisitor::visitReturn_statement(CeresParser::Return_statementContext *ctx) {
        ASSERT(ctx != nullptr);
        checkException(*ctx);

        return visit(ctx->returnStatement());
    }

    std::any
    AntlrASTGeneratorVisitor::visitExpr_statement(CeresParser::Expr_statementContext *ctx) {
        ASSERT(ctx != nullptr);
        checkException(*ctx);

        auto expr =
                std::unique_ptr<Expression>(std::any_cast<Expression *>(visit(ctx->expression())));

        return static_cast<Statement *>(
                new ExpressionStatement(getSourceSpan(*ctx), std::move(expr)));
    }

    std::any AntlrASTGeneratorVisitor::visitIf_statement(CeresParser::If_statementContext *ctx) {
        ASSERT(ctx != nullptr);
        checkException(*ctx);

        return visit(ctx->ifStatement());
    }

    std::any
    AntlrASTGeneratorVisitor::visitWhile_statement(CeresParser::While_statementContext *ctx) {
        ASSERT(ctx != nullptr);
        checkException(*ctx);

        return visit(ctx->whileStatement());
    }

    std::any AntlrASTGeneratorVisitor::visitFor_statement(CeresParser::For_statementContext *ctx) {
        ASSERT(ctx != nullptr);
        checkException(*ctx);

        return visit(ctx->forStatement());
    }

    std::any
    AntlrASTGeneratorVisitor::visitBlock_statement(CeresParser::Block_statementContext *ctx) {
        ASSERT(ctx != nullptr);
        checkException(*ctx);

        return visit(ctx->block());
    }

    std::any
    AntlrASTGeneratorVisitor::visitEmpty_statement(CeresParser::Empty_statementContext *ctx) {
        ASSERT(ctx != nullptr);
        checkException(*ctx);

        return static_cast<Statement *>(nullptr);
    }

    // TODO: When adding a new AST node, don't forget to call checkExceptions
}// namespace Ceres::AST