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
    // TODO: Check if integer literals will fit using IntLiteralExpression::doesLiteralFitInsideType()

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
    if (decl.initializerExpression != nullptr) {
        visit(*decl.initializerExpression);
        auto* extype = decl.initializerExpression->type;

        if (llvm::isa<ErrorType>(extype)) {
            if (decl.type == NotYetInferredType::get(NotYetInferredKind::VariableDeclaration)) {
                // Infer error type
                decl.type = ErrorType::get();
            }
            return;
        }

        if (decl.type == NotYetInferredType::get(NotYetInferredKind::VariableDeclaration)) {
            // Infer simple types

            switch (extype->getKind()) {

            case Type::TypeKind::UnitVoidType:
                // TODO: Maybe allow for variables of type void?
                Diagnostics::report(decl.sourceSpan, Diag::init_var_with_void, decl.id);
                decl.type = ErrorType::get();
                return;
            case Type::TypeKind::PrimitiveIntegerType:
            case Type::TypeKind::PrimitiveFloatType:
            case Type::TypeKind::FunctionType:
            case Type::TypeKind::Bool:
                decl.type = extype;
                break;
            case Type::TypeKind::NotYetInferredType: {
                /* Literals */
                auto* notYetInferredType = llvm::dyn_cast<NotYetInferredType>(extype);
                ASSERT(notYetInferredType != nullptr);

                auto maybeType = notYetInferredType->getDefaultType();
                ASSERT(maybeType.has_value());

                decl.type = maybeType.value();
                break;
            }
            case Type::TypeKind::UnresolvedType:
            case Type::TypeKind::ErrorType:
                ASSERT_NOT_REACHED();
            default:
                NOT_IMPLEMENTED();
            }
        }

        // TODO: this should always be the variable type
        auto* coerced = Type::getImplicitlyCoercedType(extype, decl.type);

        if (coerced != ErrorType::get()) {
            // Assign variable type to all members of RHS
            expandCoercion(coerced, *decl.initializerExpression);
        } else {
            Diagnostics::report(
                decl.sourceSpan, Diag::mismatched_type_on_var_decl, decl.type->toString(), extype->toString());
        }
    } else if (decl.type == NotYetInferredType::get(NotYetInferredKind::VariableDeclaration)) {
        // Variable does not have an assignment expression and doesn't have a type
        Diagnostics::report(decl.sourceSpan, Diag::type_annotation_needed, decl.id);
        decl.type = ErrorType::get();
    }
}

void TypeCheckVisitor::visitAssignmentExpression(AST::AssignmentExpression& expr)
{
    ASSERT(expr.expressionRHS != nullptr);

    // TODO: Here in the future we will have to check if is LHS
    auto* identifier = dynamic_cast<AST::IdentifierExpression*>(expr.expressionLHS.get());
    if (identifier == nullptr) {
        Log::panic("LHS of an expression is not an identifier");
    }

    auto maybe_lhs = identifier->decl;

    if (!maybe_lhs.has_value()) {
        // Error on definition of identifier
        expr.type = ErrorType::get();
        return;
    }

    auto lhs = maybe_lhs.value();

    if (lhs.getConstness().kind != Constness::NonConst) {
        Diagnostics::report(expr.sourceSpan, Diag::assign_to_const, lhs.getId());
        Diagnostics::report(lhs.getDeclarationNode()->sourceSpan, Diag::assign_to_const_note, lhs.getId());
    }

    visit(*expr.expressionRHS);

    if (llvm::isa<ErrorType>(expr.expressionRHS->type)) {
        expr.type = ErrorType::get();
        return;
    }

    auto* coerced = Type::getImplicitlyCoercedType(lhs.getType(), expr.expressionRHS->type);

    if (coerced != ErrorType::get()) {
        // Assign variable type to all members of RHS
        expandCoercion(coerced, *expr.expressionRHS);
    } else {
        Diagnostics::report(expr.sourceSpan, Diag::mismatched_assign_stmt, lhs.getType()->toString(),
            expr.expressionRHS->type->toString());
        expr.type = ErrorType::get();
        return;
    }

    expr.type = expr.expressionRHS->type;
}

void TypeCheckVisitor::visitBinaryOperationExpression(AST::BinaryOperationExpression& expr)
{
    if (expr.right != nullptr) {

        visit(*expr.right);
        visit(*expr.left);

        if (llvm::isa<ErrorType>(expr.left->type) || llvm::isa<ErrorType>(expr.right->type)) {
            expr.type = ErrorType::get();
            return;
        }

        auto* coerced = Type::getImplicitlyCoercedType(expr.right->type, expr.left->type);

        if (coerced != ErrorType::get()) {
            expandCoercion(coerced, *expr.left);
            expandCoercion(coerced, *expr.right);
        } else {
            Diagnostics::report(
                expr.sourceSpan, Diag::mismatched_expr, expr.left->type->toString(), expr.right->type->toString());
            expr.type = ErrorType::get();
            return;
        }

        auto* result = expr.op.resTy(expr.left->type);

        if (llvm::isa<ErrorType>(result)) {
            Diagnostics::report(expr.sourceSpan, Diag::mismatched_type_on_bin_op, expr.op.toString(),
                expr.left->type->toString(), expr.right->type->toString());
        }

        expr.type = result;
    }
}

void TypeCheckVisitor::visitFunctionCallExpression(AST::FunctionCallExpression& expr)
{
    ASSERT(expr.identifier != nullptr);
    visit(*expr.identifier);

    // Check that the type of the callee is a function
    FunctionType* funType = llvm::dyn_cast<FunctionType>(expr.identifier->type);
    if (funType == nullptr) {
        if (!llvm::isa<ErrorType>(expr.identifier->type)) {
            Diagnostics::report(expr.sourceSpan, Diag::calling_a_non_function_type, expr.identifier->type->toString());
        }
        expr.type = ErrorType::get();
        return;
    }

    expr.type = funType->returnType;

    for (auto i = 0; i < expr.arguments.size(); ++i) {
        visit(*expr.arguments[i]);
        auto* ty = funType->argumentTypes[i];

        if (llvm::isa<ErrorType>(ty)) {
            continue;
        }

        auto* coerced = Type::getImplicitlyCoercedType(ty, expr.arguments[i]->type);
        ASSERT(coerced != nullptr);

        if (coerced != ErrorType::get()) {
            expandCoercion(coerced, *expr.arguments[i]);
        } else {
            Diagnostics::report(expr.arguments[i]->sourceSpan, Diag::mismatched_type_on_function_call, ty->toString(),
                expr.arguments[i]->type->toString());
        }
    }
}

void TypeCheckVisitor::visitIdentifierExpression(AST::IdentifierExpression& expr)
{
    auto maybe_rhs = expr.decl;

    if (!maybe_rhs.has_value()) {
        // Error on binding identifier
        expr.type = ErrorType::get();
        return;
    }

    auto rhs = maybe_rhs.value();
    expr.type = rhs.getType();
    ASSERT(expr.type != nullptr);
}

void TypeCheckVisitor::visitReturnStatement(AST::ReturnStatement& stm)
{
    ASSERT(stm.decl.has_value());

    Type* retType;

    if (auto* funType = llvm::dyn_cast<FunctionType>(stm.decl->getType())) {
        retType = funType->returnType;
    } else {
        // Return should always point to a function
        ASSERT_NOT_REACHED();
    }

    if (stm.expr == nullptr) {
        if (!llvm::isa<VoidType>(retType)) {
            Diagnostics::report(stm.sourceSpan, Diag::empty_return_non_void_function);
        }

        return;
    }

    visit(*stm.expr);

    if (llvm::isa<ErrorType>(stm.expr->type)) {
        return;
    }

    auto* coerced = Type::getImplicitlyCoercedType(retType, stm.expr->type);

    if (coerced != ErrorType::get()) {
        // Assign variable type to all members of RHS
        expandCoercion(coerced, *stm.expr);
    } else {
        Diagnostics::report(
            stm.sourceSpan, Diag::mismatched_return_stmt, retType->toString(), stm.expr->type->toString());
    }
}

void TypeCheckVisitor::visitIfStatement(AST::IfStatement& stm)
{
    visitChildren(stm);

    if (llvm::isa<ErrorType>(stm.condition->type)) {
        return;
    }

    if (!llvm::isa<BoolType>(stm.condition->type)) {
        Diagnostics::report(
            stm.condition->sourceSpan, Diag::mismatched_type_on_if_expr, stm.condition->type->toString());
    }
}

void TypeCheckVisitor::visitForStatement(AST::ForStatement& stm)
{
    visitChildren(stm);

    if (stm.maybeConditionExpr == nullptr) {
        return;
    }

    if (llvm::isa<ErrorType>(stm.maybeConditionExpr->type)) {
        return;
    }

    if (!llvm::isa<BoolType>(stm.maybeConditionExpr->type)) {
        Diagnostics::report(stm.maybeConditionExpr->sourceSpan, Diag::mismatched_type_on_for_expr,
            stm.maybeConditionExpr->type->toString());
    }
}

void TypeCheckVisitor::visitWhileStatement(AST::WhileStatement& stm)
{
    visitChildren(stm);

    if (llvm::isa<ErrorType>(stm.condition->type)) {
        return;
    }

    if (!llvm::isa<BoolType>(stm.condition->type)) {
        Diagnostics::report(
            stm.condition->sourceSpan, Diag::mismatched_type_on_while_expr, stm.condition->type->toString());
    }
}

void TypeCheckVisitor::visitCommaExpression(AST::CommaExpression& expr)
{
    visitChildren(expr);

    expr.type = expr.expressions.back()->type;
}

void TypeCheckVisitor::visitPostfixExpression(AST::PostfixExpression& expr)
{
    visitChildren(expr);

    if (llvm::isa<ErrorType>(expr.expr->type)) {
        expr.type = ErrorType::get();
        return;
    }

    switch (expr.op) {
    case AST::PostfixOp::PostfixIncrement:
    case AST::PostfixOp::PostfixDecrement: {
        if (llvm::isa<PrimitiveIntegerType>(expr.expr->type) || llvm::isa<PrimitiveFloatType>(expr.expr->type)) {
            expr.type = expr.expr->type;
        } else {
            Diagnostics::report(expr.sourceSpan, Diag::mismatched_type_on_postfix_operator, postfixOpToString(expr.op),
                "integer or float", expr.expr->type->toString());
            expr.type = ErrorType::get();
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

    if (llvm::isa<ErrorType>(expr.expr->type)) {
        expr.type = ErrorType::get();
        return;
    }

    switch (expr.op) {
    case AST::PrefixOp::PrefixIncrement:
    case AST::PrefixOp::PrefixDecrement:
    case AST::PrefixOp::UnaryPlus:
    case AST::PrefixOp::UnaryMinus: {
        if (llvm::isa<PrimitiveIntegerType>(expr.expr->type) || llvm::isa<PrimitiveFloatType>(expr.expr->type)) {
            expr.type = expr.expr->type;
        } else {
            Diagnostics::report(expr.sourceSpan, Diag::mismatched_type_on_prefix_operator, prefixOpToString(expr.op),
                "integer or float", expr.expr->type->toString());
            expr.type = ErrorType::get();
        }
        break;
    }
    case AST::PrefixOp::UnaryLogicalNot: {
        if (llvm::isa<BoolType>(expr.expr->type)) {
            expr.type = expr.expr->type;
        } else {
            Diagnostics::report(expr.sourceSpan, Diag::mismatched_type_on_prefix_operator, prefixOpToString(expr.op),
                "bool", expr.expr->type->toString());
            expr.type = ErrorType::get();
        }
        break;
    }
    case AST::PrefixOp::UnaryBitwiseNot: {
        if (llvm::isa<PrimitiveIntegerType>(expr.expr->type)) {
            expr.type = expr.expr->type;
        } else {
            Diagnostics::report(expr.sourceSpan, Diag::mismatched_type_on_prefix_operator, prefixOpToString(expr.op),
                "integer", expr.expr->type->toString());
            expr.type = ErrorType::get();
        }
        break;
    }
    default:
        ASSERT_NOT_REACHED();
    }
}

void TypeCheckVisitor::visitCastExpression(AST::CastExpression& expr)
{
    visitChildren(expr);

    NOT_IMPLEMENTED();
}

} // namespace Ceres::Typing
