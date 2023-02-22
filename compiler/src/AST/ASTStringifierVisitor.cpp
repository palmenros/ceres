#include "ASTStringifierVisitor.h"
#include "../Typing/BinaryOperation.h"
#include "../utils/log.hpp"
#include "spdlog/fmt/fmt.h"
#include <iterator>

namespace Ceres::AST {
std::string ASTStringifierVisitor::doVisitAssignmentExpression(AssignmentExpression& expr)
{
    std::string opStr;

    if (!expr.binaryOp.has_value()) {
        opStr = "=";
    } else {
        switch (expr.binaryOp.value().kind) {
        case Typing::BinaryOperation::Mult:
            opStr = "*=";
            break;
        case Typing::BinaryOperation::Div:
            opStr = "/=";
            break;
        case Typing::BinaryOperation::Modulo:
            opStr = "%=";
            break;
        case Typing::BinaryOperation::Sum:
            opStr = "+=";
            break;
        case Typing::BinaryOperation::Subtraction:
            opStr = "-=";
            break;
        case Typing::BinaryOperation::BitshiftLeft:
            opStr = "<<=";
            break;
        case Typing::BinaryOperation::BitshiftRight:
            opStr = ">>=";
            break;
        case Typing::BinaryOperation::BitwiseAnd:
            opStr = "&=";
            break;
        case Typing::BinaryOperation::BitwiseOr:
            opStr = "|=";
            break;
        case Typing::BinaryOperation::BitwiseXor:
            opStr = "^=";
            break;
        default:
            NOT_IMPLEMENTED();
        }
    }

    return fmt::format("(AssignmentExpression op='{}' lhs='{}' rhs={})", opStr, visit(*expr.expressionLHS),
        visit(*expr.expressionRHS));
}

std::string ASTStringifierVisitor::doVisitBinaryOperationExpression(BinaryOperationExpression& expr)
{
    std::string opStr;

    switch (expr.op.kind) {
    case Typing::BinaryOperation::Mult:
        opStr = "*";
        break;
    case Typing::BinaryOperation::Div:
        opStr = "/";
        break;
    case Typing::BinaryOperation::Modulo:
        opStr = "%";
        break;
    case Typing::BinaryOperation::Sum:
        opStr = "+";
        break;
    case Typing::BinaryOperation::Subtraction:
        opStr = "-";
        break;
    case Typing::BinaryOperation::BitshiftLeft:
        opStr = "<<";
        break;
    case Typing::BinaryOperation::BitshiftRight:
        opStr = ">>";
        break;
    case Typing::BinaryOperation::LessOrEqual:
        opStr = "<=";
        break;
    case Typing::BinaryOperation::GreaterOrEqual:
        opStr = ">=";
        break;
    case Typing::BinaryOperation::GreaterThan:
        opStr = ">";
        break;
    case Typing::BinaryOperation::LessThan:
        opStr = "<";
        break;
    case Typing::BinaryOperation::BitwiseAnd:
        opStr = "&";
        break;
    case Typing::BinaryOperation::BitwiseOr:
        opStr = "|";
        break;
    case Typing::BinaryOperation::BitwiseXor:
        opStr = "^";
        break;
    case Typing::BinaryOperation::Equals:
        opStr = "==";
        break;
    case Typing::BinaryOperation::NotEquals:
        opStr = "!=";
        break;
    case Typing::BinaryOperation::LogicalAnd:
        opStr = "&&";
        break;
    case Typing::BinaryOperation::LogicalOr:
        opStr = "||";
        break;
    default:
        NOT_IMPLEMENTED();
        break;
    }

    return fmt::format("(BinaryExpression BinOp='{}' lhs='{}' rhs={})", opStr, visit(*expr.left), visit(*expr.right));
}

std::string ASTStringifierVisitor::doVisitBlockStatement(BlockStatement& block)
{
    std::string res;
    bool first = true;

    for (auto const& stm : block.statements) {
        if (first) {
            first = false;
        } else {
            res += " ";
        }
        res += visit(*stm);
    }

    return fmt::format("(BlockStatement '{}')", res);
}

std::string ASTStringifierVisitor::doVisitBoolLiteral(BoolLiteralExpression& lit)
{
    return fmt::format("(BoolLiteralExpression {})", BoolLiteralExpression::toStringBoolLiteralValue(lit.value));
}

std::string ASTStringifierVisitor::doVisitCommaExpression(CommaExpression& expr)
{
    std::string res = "(CommaExpression ";
    bool first = true;
    for (auto& childPtr : expr.getChildren()) {
        if (first) {
            first = false;
        } else {
            res += " ";
        }
        res += visit(*childPtr);
    }
    return res + ")";
}

std::string ASTStringifierVisitor::doVisitCompilationUnit(CompilationUnit& unit)
{
    std::string res;
    bool first = true;
    for (auto& childPtr : unit.getChildren()) {
        if (first) {
            first = false;
        } else {
            res += " ";
        }
        res += visit(*childPtr);
    }
    return fmt::format("(CompilationUnit '{}')", res);
}

std::string ASTStringifierVisitor::doVisitExpressionStatement(ExpressionStatement& stm)
{
    return fmt::format("(ExpressionStatement '{}')", visit(*stm.expression));
}

std::string ASTStringifierVisitor::doVisitFloatLiteralExpression(FloatLiteralExpression& expr)
{
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

std::string ASTStringifierVisitor::doVisitForStatement(ForStatement& stm)
{
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

std::string ASTStringifierVisitor::doVisitFunctionCallExpression(FunctionCallExpression& expr)
{
    std::string args;
    bool first = true;
    for (auto& arg : expr.arguments) {
        if (first) {
            first = false;
        } else {
            args += ", ";
        }
        args += visit(*arg);
    }

    return fmt::format("(FunctionCallExpression id='{}' args='{}')", expr.identifier->identifier, args);
}

std::string ASTStringifierVisitor::doVisitFunctionDefinition(FunctionDefinition& def)
{
    std::string paramsString;
    bool first = true;
    for (auto const& param : def.parameters) {
        std::string paramString = fmt::format("{} {}: {}",
            param.constness.kind == Typing::Constness::NonConst ? "var" : "const", param.id, param.type->toString());
        if (first) {
            first = false;
        } else {
            paramsString += ", ";
        }
        paramsString += paramString;
    }

    return fmt::format(
        "(FunctionDefinition id='{}', params='{}' body='{}')", def.id, paramsString, visit(*def.block));
}

std::string ASTStringifierVisitor::doVisitFunctionDeclaration(FunctionDeclaration& dec)
{
    std::string paramsString;
    bool first = true;
    for (auto const& param : dec.parameters) {
        std::string paramString = fmt::format("{} {}: {}",
            param.constness.kind == Typing::Constness::NonConst ? "var" : "const", param.id, param.type->toString());
        if (first) {
            first = false;
        } else {
            paramsString += ", ";
        }
        paramsString += paramString;
    }

    return fmt::format("(FunctionDeclaration id='{}', params='{}')", dec.functionName, paramsString);
}

std::string ASTStringifierVisitor::doVisitIdentifierExpression(IdentifierExpression& expr)
{
    return fmt::format("(IdentifierExpression id='{}')", expr.identifier);
}

std::string ASTStringifierVisitor::doVisitIfStatement(IfStatement& stm)
{
    if (stm.maybeElseStatement != nullptr) {
        return fmt::format("(IfStatement cond='{}' body='{}' else='{}')", visit(*stm.condition), visit(*stm.thenBlock),
            visit(*stm.maybeElseStatement));
    }

    return fmt::format("(IfStatement cond='{}' body='{}')", visit(*stm.condition), visit(*stm.thenBlock));
}

std::string ASTStringifierVisitor::doVisitIntLiteralExpression(IntLiteralExpression& expr)
{
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

    return fmt::format("(IntLiteralExpression base='{}' text='{}' type='{}')", baseString, expr.str, typeString);
}

std::string ASTStringifierVisitor::doVisitPostfixExpression(PostfixExpression& expr)
{
    std::string opStr = postfixOpToString(expr.op);
    return fmt::format("(PostfixExpression op='{}' expr='{}')", opStr, visit(*expr.expr));
}

std::string ASTStringifierVisitor::doVisitPrefixExpression(PrefixExpression& expr)
{
    std::string opStr = prefixOpToString(expr.op);
    return fmt::format("(PrefixExpression op='{}' expr='{}')", opStr, visit(*expr.expr));
}

std::string ASTStringifierVisitor::doVisitReturnStatement(ReturnStatement& stm)
{
    std::string expr;
    if (stm.expr != nullptr) {
        expr = visit(*stm.expr);
    }

    return fmt::format("(ReturnStatement expr='{}')", expr);
}

std::string ASTStringifierVisitor::doVisitVariableDeclaration(VariableDeclaration& decl)
{
    std::string vis = decl.visibility.kind == Typing::VariableVisibility::Private ? "private" : "public";
    std::string scope = decl.scope == VariableScope::Local ? "local" : "global";
    std::string constn = decl.constness.kind == Typing::Constness::Const ? "const" : "var";

    if (decl.initializerExpression != nullptr) {
        return fmt::format("(VariableDeclaration vis='{}' scope='{}' const='{}' type='{}' "
                           "id='{}' expr='{}')",
            vis, scope, constn, decl.type->toString(), decl.id, visit(*decl.initializerExpression));
    }

    return fmt::format("(VariableDeclaration vis='{}' scope='{}' const='{}' "
                       "type='{}' id='{}')",
        vis, scope, constn, decl.type->toString(), decl.id);
}

std::string ASTStringifierVisitor::doVisitWhileStatement(WhileStatement& stm)
{
    return fmt::format("(WhileStatement cond='{}' body='{}')", visit(*stm.condition), visit(*stm.body));
}
} // namespace Ceres::AST