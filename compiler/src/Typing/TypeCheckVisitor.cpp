#include "TypeCheckVisitor.h"
#include "../Binding/Scope.h"
#include "../Diagnostics/Diagnostics.h"
#include "Type.h"
#include <optional>

namespace Ceres::Typing {
    void expandCoercion(Type *coerced, Expression &lhs) {
        // Assign variable type to all members of expression
        if (lhs.type != coerced) {
            lhs.type = coerced;
            for (const auto e: lhs.getChildren()) {
                auto ee = static_cast<AST::Expression *>(e);
                ee->type = coerced;
            }
        }
    }

    // Reconstruct scopes
    void TypeCheckVisitor::visitCompilationUnit(CompilationUnit &unit) {
        currentScope = &unit.scope.value();
        visitChildren(unit);
    }

    void TypeCheckVisitor::visitBlockStatement(BlockStatement &stm) {
        currentScope = &stm.scope.value();
        visitChildren(stm);
        currentScope = currentScope->getEnclosingScope();
    }

    void TypeCheckVisitor::visitFunctionDefinition(FunctionDefinition &def) {
        currentScope = &def.block->scope.value();
        currentFunction = &def;
        visitChildren(*def.block);
        currentScope = currentScope->getEnclosingScope();
        currentFunction = nullptr;
    }

    // Type check

    void TypeCheckVisitor::visitVariableDeclaration(VariableDeclaration &decl) {
        // TODO: Dont infer types yet
        ASSERT(decl.type != NotYetInferredType::get(NotYetInferredKind::VariableDeclaration));

        if (decl.initializerExpression != nullptr) {
            visit(*decl.initializerExpression);
            auto extype = decl.initializerExpression->type;

            // TODO: this should always be the variable type
            auto coerced = Type::getImplicitlyCoercedType(extype, decl.type);

            if (coerced != ErrorType::get()) {
                // Assign variable type to all members of RHS
                expandCoercion(coerced, *decl.initializerExpression);
            } else {
                // TODO: Better errors
                Diagnostics::report(decl.sourceSpan, Diag::mismatch_type_error,
                                    decl.type->toString(), extype->toString());
                Log::panic("Useless panic");
            }
        }
    }

    void TypeCheckVisitor::visitAssignmentExpression(AssignmentExpression &expr) {
        if (expr.expressionRHS != nullptr) {
            visit(*expr.expressionRHS);
            auto lhs = currentScope->resolve(expr.identifierLHS);

            auto coerced = Type::getImplicitlyCoercedType(lhs.getType(), expr.expressionRHS->type);

            if (coerced != ErrorType::get()) {
                // Assign variable type to all members of RHS
                expandCoercion(coerced, *expr.expressionRHS);
            } else {
                Diagnostics::report(expr.sourceSpan, Diag::mismatch_type_error,
                                    lhs.getType()->toString(),
                                    expr.expressionRHS->type->toString());
                Log::panic("Useless panic");
            }

            expr.type = expr.expressionRHS->type;
        }
    }

    void TypeCheckVisitor::visitBinaryOperationExpression(BinaryOperationExpression &expr) {
        if (expr.right != nullptr) {
            visit(*expr.right);
            visit(*expr.left);

            auto coerced = Type::getImplicitlyCoercedType(expr.right->type, expr.left->type);

            if (coerced != ErrorType::get()) {
                expandCoercion(coerced, *expr.left);
                expandCoercion(coerced, *expr.right);
            } else {
                Diagnostics::report(expr.sourceSpan, Diag::mismatch_type_error,
                                    expr.left->type->toString(), expr.right->type->toString());
                Log::panic("Useless panic");
            }

            expr.type = expr.right->type;
        }
    }

    void TypeCheckVisitor::visitFunctionCallExpression(FunctionCallExpression &expr) {
        auto rhs = currentScope->resolve(expr.functionIdentifier);
        expr.type = rhs.getType();
    }

    void TypeCheckVisitor::visitIdentifierExpression(IdentifierExpression &expr) {
        auto rhs = currentScope->resolve(expr.identifier);
        expr.type = rhs.getType();
    }

    void TypeCheckVisitor::visitReturnStatement(ReturnStatement &stm) {
        auto retType = currentFunction->returnType;
        visit(*stm.expr);

        auto coerced = Type::getImplicitlyCoercedType(retType, stm.expr->type);

        if (coerced != ErrorType::get()) {
            // Assign variable type to all members of RHS
            expandCoercion(coerced, *stm.expr);
        } else {
            Diagnostics::report(stm.sourceSpan, Diag::mismatch_type_error, retType->toString(),
                                stm.expr->type->toString());
            Log::panic("Useless panic");
        }
    }


}// namespace Ceres::Typing
