#include "TypeCheckVisitor.h"
#include "../Binding/Scope.h"
#include "Type.h"
#include <optional>

namespace Ceres::Typing {
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
        visitChildren(*def.block);
        currentScope = currentScope->getEnclosingScope();
    }

    // Type check

    void TypeCheckVisitor::visitVariableDeclaration(VariableDeclaration &decl) {
        // TODO: Dont infer types yet
        ASSERT(decl.type != NotYetInferredType::get(NotYetInferredKind::VariableDeclaration));

        if (decl.initializerExpression != nullptr) {
            visit(*decl.initializerExpression);
            auto extype = &decl.initializerExpression->type;

            if (*extype == NotYetInferredType::get(NotYetInferredKind::NumberLiteral)) {
                // TODO: how can we check for compatible type conversions?
                Log::info("Encountered untyped literal");

                // Assign variable type to all members of RHS
                decl.initializerExpression->type = decl.type;
                for (const auto e: decl.initializerExpression->getChildren()) {
                    auto ee = static_cast<AST::Expression *>(e);
                    ee->type = decl.type;
                }
            }

            if (decl.type != *extype) {
                // TODO: implement print on span, with line and char
                Log::panic("Variable decl mismatched types on line: ");
            }
        }
    }

    void TypeCheckVisitor::visitAssignmentExpression(AssignmentExpression &expr) {
        if (expr.expressionRHS != nullptr) {
            visit(*expr.expressionRHS);
            auto rhs = currentScope->resolve(expr.identifierLHS);

            if (rhs.getType() != expr.expressionRHS->type) {
                // TODO: implement print on span, with line and char
                Log::panic("Mismatched types on line: ");
            }

            expr.type = expr.expressionRHS->type;
        } else {
            // TODO: maybe impossible by syntax
            Log::panic("Empty assignment");
        }
    }

    void TypeCheckVisitor::visitBinaryOperationExpression(BinaryOperationExpression &expr) {
        if (expr.right != nullptr) {
            visit(*expr.right);
            visit(*expr.left);

            if (expr.left->type != expr.right->type) {
                // TODO: implement print on span, with line and char
                Log::panic("Mismatched types on line: ");
            }

            expr.type = expr.right->type;
        } else {
            // TODO: maybe impossible by syntax
            Log::panic("Empty assignment");
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


}// namespace Ceres::Typing
