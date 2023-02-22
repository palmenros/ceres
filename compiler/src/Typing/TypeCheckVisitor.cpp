#include "TypeCheckVisitor.h"
#include "../Binding/Scope.h"
#include "../Diagnostics/Diagnostics.h"
#include "BinaryOperation.h"
#include "Type.h"
#include <cstddef>
#include <llvm/Support/Casting.h>
#include <optional>
#include <spdlog/fmt/bundled/core.h>

namespace Ceres::Typing {

// TODO: do this right, when we implement inference
void expandCoercion(Type* coerced, AST::Expression& lhs)
{
    // Assign variable type to all members of expression
    if (lhs.type != coerced) {
        lhs.type = coerced;
        for (auto* const e : lhs.getChildren()) {
            auto* ee = dynamic_cast<AST::Expression*>(e);
            ee->type = coerced;
        }
    }
}

void TypeCheckVisitor::visitVariableDeclaration(AST::VariableDeclaration& decl)
{
    // TODO: Dont infer types yet
    if (decl.type == NotYetInferredType::get(NotYetInferredKind::VariableDeclaration)) {
        Log::panic("We dont have type inference yet");
    }

    if (decl.initializerExpression != nullptr) {
        visit(*decl.initializerExpression);
        auto* extype = decl.initializerExpression->type;

        // TODO: this should always be the variable type
        auto* coerced = Type::getImplicitlyCoercedType(extype, decl.type);

        if (coerced != ErrorType::get()) {
            // Assign variable type to all members of RHS
            expandCoercion(coerced, *decl.initializerExpression);
        } else {
            // TODO: Better errors
            Diagnostics::report(
                decl.sourceSpan, Diag::mismatched_type_on_var_decl, decl.type->toString(), extype->toString());
            Log::panic("Useless panic");
        }
    }
}

void TypeCheckVisitor::visitAssignmentExpression(AST::AssignmentExpression& expr)
{
    ASSERT(expr.expressionRHS != nullptr);

    auto* identifier = dynamic_cast<AST::IdentifierExpression*>(expr.expressionLHS.get());
    if (identifier == nullptr) {
        Log::panic("LHS of an expression is not an identifier");
    }

    auto maybe_lhs = identifier->decl;
    ASSERT(maybe_lhs.has_value());

    auto lhs = maybe_lhs.value();

    if (lhs.getConstness().kind != Constness::NonConst) {
        Diagnostics::report(expr.sourceSpan, Diag::assign_to_const, lhs.getId());
    }

    visit(*expr.expressionRHS);

    auto* coerced = Type::getImplicitlyCoercedType(lhs.getType(), expr.expressionRHS->type);

    if (coerced != ErrorType::get()) {
        // Assign variable type to all members of RHS
        expandCoercion(coerced, *expr.expressionRHS);
    } else {
        Diagnostics::report(expr.sourceSpan, Diag::mismatched_assign_stmt, lhs.getType()->toString(),
            expr.expressionRHS->type->toString());
        Log::panic("Useless panic");
    }

    expr.type = expr.expressionRHS->type;
}

void TypeCheckVisitor::visitBinaryOperationExpression(AST::BinaryOperationExpression& expr)
{
    if (expr.right != nullptr) {

        visit(*expr.right);
        visit(*expr.left);

        auto* coerced = Type::getImplicitlyCoercedType(expr.right->type, expr.left->type);

        if (coerced != ErrorType::get()) {
            expandCoercion(coerced, *expr.left);
            expandCoercion(coerced, *expr.right);
        } else {
            Diagnostics::report(
                expr.sourceSpan, Diag::mismatched_expr, expr.left->type->toString(), expr.right->type->toString());
            Log::panic("Useless panic");
        }

        auto* result = expr.op.resTy(expr.left->type);

        if (llvm::isa<ErrorType>(result)) {
            Diagnostics::report(expr.sourceSpan, Diag::mismatched_type_on_bin_op, expr.op.toString(),
                expr.left->type->toString(), expr.right->type->toString());
            Log::panic("Useless panic");
        }

        expr.type = result;
    }
}

void TypeCheckVisitor::visitFunctionCallExpression(AST::FunctionCallExpression& expr)
{
    auto maybe_rhs = expr.identifier->decl;
    ASSERT(maybe_rhs.has_value());

    auto rhs = maybe_rhs.value();

    expr.type = rhs.getType();
    auto* decl = dynamic_cast<AST::FunctionDefinition*>(rhs.getDeclarationNode());
    ASSERT(decl != nullptr);

    for (auto i = 0; i < expr.arguments.size(); ++i) {
        visit(*expr.arguments[i]);
        auto* ty = decl->parameters[i].type;

        auto* coerced = Type::getImplicitlyCoercedType(ty, expr.arguments[i]->type);
        ASSERT(coerced != nullptr);

        if (coerced != ErrorType::get()) {
            expandCoercion(coerced, *expr.arguments[i]);
        } else {
            Diagnostics::report(expr.arguments[i]->sourceSpan, Diag::mismatched_type_on_function_call, ty->toString(),
                expr.arguments[i]->type->toString());
            Log::panic("Useless panic");
        }
    }
}

void TypeCheckVisitor::visitIdentifierExpression(AST::IdentifierExpression& expr)
{
    auto maybe_rhs = expr.decl;
    ASSERT(maybe_rhs.has_value());

    auto rhs = maybe_rhs.value();

    expr.type = rhs.getType();
}

void TypeCheckVisitor::visitReturnStatement(AST::ReturnStatement& stm)
{
    ASSERT(stm.decl.has_value());
    auto* retType = stm.decl->getType();

    if (stm.expr == nullptr) {
        if (!llvm::isa<VoidType>(retType)) {
            Diagnostics::report(stm.sourceSpan, Diag::empty_return_non_void_function);
            Log::panic("Useless panic");
        }

        return;
    }

    visit(*stm.expr);

    auto* coerced = Type::getImplicitlyCoercedType(retType, stm.expr->type);

    if (coerced != ErrorType::get()) {
        // Assign variable type to all members of RHS
        expandCoercion(coerced, *stm.expr);
    } else {
        Diagnostics::report(
            stm.sourceSpan, Diag::mismatched_return_stmt, retType->toString(), stm.expr->type->toString());
        Log::panic("Useless panic");
    }
}

void TypeCheckVisitor::visitIfStatement(AST::IfStatement& stm)
{
    visitChildren(stm);

    if (!llvm::isa<BoolType>(stm.condition->type)) {
        Diagnostics::report(stm.condition->sourceSpan, Diag::mismatched_type_on_if_expr, stm.condition->type->toString());
        Log::panic("Useless panic");
    }
}

void TypeCheckVisitor::visitForStatement(AST::ForStatement& stm) {
    visitChildren(stm);

    if(stm.maybeConditionExpr == nullptr) {
        return;
    }

    if (!llvm::isa<BoolType>(stm.maybeConditionExpr->type)) {
        Diagnostics::report(stm.maybeConditionExpr->sourceSpan, Diag::mismatched_type_on_for_expr, stm.maybeConditionExpr->type->toString());
        Log::panic("Useless panic");
    }
}

void TypeCheckVisitor::visitWhileStatement(AST::WhileStatement& stm) {
    visitChildren(stm);

    if (!llvm::isa<BoolType>(stm.condition->type)) {
        Diagnostics::report(stm.condition->sourceSpan, Diag::mismatched_type_on_while_expr, stm.condition->type->toString());
        Log::panic("Useless panic");
    }
}

void TypeCheckVisitor::visitCommaExpression(AST::CommaExpression& expr) {
    visitChildren(expr);

    expr.type = expr.expressions.back()->type;
}

void TypeCheckVisitor::visitPostfixExpression(AST::PostfixExpression& expr)
{
    visitChildren(expr);

    switch(expr.op) {
    case AST::PostfixOp::PostfixIncrement:
    case AST::PostfixOp::PostfixDecrement: {
        if (llvm::isa<PrimitiveIntegerType>(expr.expr->type) || llvm::isa<PrimitiveFloatType>(expr.expr->type)) {
            expr.type = expr.expr->type;
        } else {
            Diagnostics::report(expr.sourceSpan, Diag::mismatched_type_on_postfix_operator, postfixOpToString(expr.op), "integer or float", expr.expr->type->toString());
            Log::panic("Useless panic");
        }
        break;
    }
    default:
        ASSERT_NOT_REACHED();
    }
}

void TypeCheckVisitor::visitPrefixExpression(AST::PrefixExpression& expr)
{
    visitChildren(expr);

    switch(expr.op) {
    case AST::PrefixOp::PrefixIncrement:
    case AST::PrefixOp::PrefixDecrement:
    case AST::PrefixOp::UnaryPlus:
    case AST::PrefixOp::UnaryMinus: {
        if (llvm::isa<PrimitiveIntegerType>(expr.expr->type) || llvm::isa<PrimitiveFloatType>(expr.expr->type)) {
            expr.type = expr.expr->type;
        } else {
            Diagnostics::report(expr.sourceSpan, Diag::mismatched_type_on_prefix_operator, prefixOpToString(expr.op), "integer or float", expr.expr->type->toString());
            Log::panic("Useless panic");
        }
        break;
    }
    case AST::PrefixOp::UnaryLogicalNot: {
        if (llvm::isa<BoolType>(expr.expr->type)) {
            expr.type = expr.expr->type;
        } else {
            Diagnostics::report(expr.sourceSpan, Diag::mismatched_type_on_prefix_operator, prefixOpToString(expr.op), "bool",  expr.expr->type->toString());
            Log::panic("Useless panic");
        }
        break;
    }
    case AST::PrefixOp::UnaryBitwiseNot: {
        if (llvm::isa<PrimitiveIntegerType>(expr.expr->type)) {
            expr.type = expr.expr->type;
        } else {
            Diagnostics::report(expr.sourceSpan, Diag::mismatched_type_on_prefix_operator, prefixOpToString(expr.op), "integer", expr.expr->type->toString());
            Log::panic("Useless panic");
        }
        break;
    }
    default:
        ASSERT_NOT_REACHED();
    }
}

} // namespace Ceres::Typing
