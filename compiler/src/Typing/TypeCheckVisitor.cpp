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
Type* binOpResult(BinaryOperation op, Type* lhs, Type* rhs)
{
    // Should already be of the same type
    // if (lhs != rhs) {
    //     return ErrorType::get();
    // }

    auto* typeInt = dynamic_cast<PrimitiveIntegerType*>(lhs);
    if (typeInt != nullptr) {
        switch (op.kind) {
        case BinaryOperation::Mult:
        case BinaryOperation::Div:
        case BinaryOperation::Sum:
        case BinaryOperation::Subtraction:
        case BinaryOperation::Modulo:
        case BinaryOperation::BitwiseAnd:
        case BinaryOperation::BitwiseOr:
        case BinaryOperation::BitwiseXor:
        case BinaryOperation::BitshiftLeft:
        case BinaryOperation::BitshiftRight: {
            return typeInt;
        }
        case BinaryOperation::LessOrEqual:
        case BinaryOperation::GreaterOrEqual:
        case BinaryOperation::GreaterThan:
        case BinaryOperation::LessThan:
        case BinaryOperation::Equals:
        case BinaryOperation::NotEquals: {
            return BoolType::get();
        }
        default:
            // TODO: print pretty error
            break;
        }
    }

    auto* typeB = dynamic_cast<BoolType*>(lhs);
    if (typeB != nullptr) {
        switch (op.kind) {
        case BinaryOperation::Equals:
        case BinaryOperation::NotEquals:
        case BinaryOperation::LogicalAnd:
        case BinaryOperation::LogicalOr: {
            return BoolType::get();
        }
        default:
            // TODO: print pretty error
            break;
        }
    }

    auto* typeF = dynamic_cast<NotYetInferredType*>(lhs);
    if (typeF != nullptr) {
        switch (op.kind) {
        case BinaryOperation::Mult:
        case BinaryOperation::Div:
        case BinaryOperation::Sum:
        case BinaryOperation::Subtraction: {
            return typeF;
        }
        // TODO: are floats actually comparable?
        case BinaryOperation::LessOrEqual:
        case BinaryOperation::GreaterOrEqual:
        case BinaryOperation::GreaterThan:
        case BinaryOperation::LessThan:
        case BinaryOperation::Equals:
        case BinaryOperation::NotEquals: {
            return BoolType::get();
        }
        default:
            // TODO: print pretty error
            break;
        }
    }

    Log::panic("Could not resolve binary expr type");
}

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

        auto* result = binOpResult(expr.op, expr.left->type, expr.right->type);
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
} // namespace Ceres::Typing
