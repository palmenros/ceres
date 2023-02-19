#include "TypeCheckVisitor.h"
#include "../Binding/Scope.h"
#include "../Diagnostics/Diagnostics.h"
#include "Type.h"
#include <optional>

namespace Ceres::Typing {
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
    ASSERT(def.block->scope.has_value());
    currentScope = &def.block->scope.value();
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

        expr.type = expr.right->type;
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
