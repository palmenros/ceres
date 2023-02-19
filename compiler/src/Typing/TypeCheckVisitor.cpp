#include "TypeCheckVisitor.h"
#include "../Binding/Scope.h"
#include "../Diagnostics/Diagnostics.h"
#include "Type.h"
#include <optional>

namespace Ceres::Typing {
Type* binOpResult(AST::BinaryOp op, Type* lhs, Type* rhs)
{
    // Should already be of the same type
    // if (lhs != rhs) {
    //     return ErrorType::get();
    // }

    auto* type = dynamic_cast<PrimitiveScalarType*>(lhs);
    if (type != nullptr) {
        switch (op) {
        case AST::BinaryOp::Mult:
        case AST::BinaryOp::Div:
        case AST::BinaryOp::Sum:
        case AST::BinaryOp::Subtraction: {
            if (PrimitiveScalarType::isInteger(type) or PrimitiveScalarType::isFloating(type)) {
                return type;
            }
            break;
        }
        case AST::BinaryOp::Modulo:
        case AST::BinaryOp::BitwiseAnd:
        case AST::BinaryOp::BitwiseOr:
        case AST::BinaryOp::BitwiseXor:
        case AST::BinaryOp::BitshiftLeft:
        case AST::BinaryOp::BitshiftRight: {
            if (PrimitiveScalarType::isInteger(type)) {
                return type;
            }
            break;
        }
        case AST::BinaryOp::LessOrEqual:
        case AST::BinaryOp::GreaterOrEqual:
        case AST::BinaryOp::GreaterThan:
        case AST::BinaryOp::LessThan:
        case AST::BinaryOp::Equals:
        case AST::BinaryOp::NotEquals: {
            if (PrimitiveScalarType::isInteger(type) or PrimitiveScalarType::isFloating(type)) {
                return PrimitiveScalarType::get(PrimitiveKind::BOOL);
            }
            break;
        }
        case AST::BinaryOp::LogicalAnd:
        case AST::BinaryOp::LogicalOr: {
            if (PrimitiveScalarType::isBool(type)) {
                return type;
            }
            break;
        }
        }
    }
    auto* type2 = dynamic_cast<NotYetInferredType*>(lhs);
    if (type2 != nullptr) {
        if (type2 != NotYetInferredType::get(NotYetInferredKind::NumberLiteral)) {
            Log::panic("Expected number literal, got something else");
        }

        // TODO: divide number literal in integer literal, floating literal and bool literal
        switch (op) {
        case AST::BinaryOp::Mult:
        case AST::BinaryOp::Div:
        case AST::BinaryOp::Sum:
        case AST::BinaryOp::Subtraction: {
            // TODO: if literal integer or float
            break;
        }
        case AST::BinaryOp::Modulo:
        case AST::BinaryOp::BitwiseAnd:
        case AST::BinaryOp::BitwiseOr:
        case AST::BinaryOp::BitwiseXor:
        case AST::BinaryOp::BitshiftLeft:
        case AST::BinaryOp::BitshiftRight: {
            // TODO: if literal integer
            break;
        }
        case AST::BinaryOp::LessOrEqual:
        case AST::BinaryOp::GreaterOrEqual:
        case AST::BinaryOp::GreaterThan:
        case AST::BinaryOp::LessThan:
        case AST::BinaryOp::Equals:
        case AST::BinaryOp::NotEquals: {
            // TODO: if literal integer or float
            break;
        }
        case AST::BinaryOp::LogicalAnd:
        case AST::BinaryOp::LogicalOr: {
            // TODO: if literal bool
            break;
        }
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

// Reconstruct scopes
void TypeCheckVisitor::visitCompilationUnit(AST::CompilationUnit& unit)
{
    ASSERT(unit.scope.has_value());
    currentScope = &unit.scope.value();
    visitChildren(unit);
}

void TypeCheckVisitor::visitBlockStatement(AST::BlockStatement& stm)
{
    ASSERT(stm.scope.has_value());
    currentScope = &stm.scope.value();
    visitChildren(stm);
    currentScope = currentScope->getEnclosingScope();
}

void TypeCheckVisitor::visitFunctionDefinition(AST::FunctionDefinition& def)
{
    auto* scope = &def.block->scope;
    ASSERT(scope->has_value());
    currentScope = &scope->value();
    currentFunction = &def;
    visitChildren(*def.block);
    currentScope = currentScope->getEnclosingScope();
    currentFunction = nullptr;
}

// Type check

void TypeCheckVisitor::visitVariableDeclaration(AST::VariableDeclaration& decl)
{
    // TODO: Dont infer types yet
    ASSERT(decl.type != NotYetInferredType::get(NotYetInferredKind::VariableDeclaration));

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
    // Cant happen
    if (expr.expressionRHS == nullptr) {
        Log::panic("Assignment with no body");
    }

    visit(*expr.expressionRHS);
    auto lhs = currentScope->resolve(expr.identifierLHS);

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
    auto rhs = currentScope->resolve(expr.functionIdentifier);
    expr.type = rhs.getType();
    auto* decl = dynamic_cast<AST::FunctionDefinition*>(rhs.getDeclarationNode());

    for (auto i = 0; i < expr.arguments.size(); ++i) {
        visit(*expr.arguments[i]);
        auto* ty = decl->parameters[i].type;

        auto* coerced = Type::getImplicitlyCoercedType(ty, expr.arguments[i]->type);

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
    auto rhs = currentScope->resolve(expr.identifier);
    expr.type = rhs.getType();
}

void TypeCheckVisitor::visitReturnStatement(AST::ReturnStatement& stm)
{
    auto* retType = currentFunction->returnType;
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
