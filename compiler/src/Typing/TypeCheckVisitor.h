#ifndef COMPILER_TYPECHECKVISITOR_H
#define COMPILER_TYPECHECKVISITOR_H

#include "../AST/ASTVisitor.h"
#include "../Binding/Scope.h"

namespace Ceres::Typing {
using namespace AST;

// TODO: The binding visitor is only the first pass of the binding. It should
// embed all the
//          the declarations in the necessary scopes so the second pass (that I
//          think can and should be combined with type-inference and
//          type-checking) can reference the scopes for the needed symbols.

class TypeCheckVisitor : public ASTVisitor {
public:
    Binding::Scope *currentScope = nullptr;
    AST::FunctionDefinition *currentFunction = nullptr;

    // Reconstruct scopes
    void visitCompilationUnit(CompilationUnit &unit) override;
    void visitBlockStatement(BlockStatement &stm) override;
    void visitFunctionDefinition(FunctionDefinition &def) override;

    // Type check
    void visitVariableDeclaration(VariableDeclaration &decl) override;
    void visitAssignmentExpression(AssignmentExpression &expr) override;
    void
    visitBinaryOperationExpression(BinaryOperationExpression &expr) override;
    // TODO:
    // void visitCommaExpression(CommaExpression &expr) override;
    void visitFunctionCallExpression(FunctionCallExpression &expr) override;
    void visitIdentifierExpression(IdentifierExpression &expr) override;
    void visitReturnStatement(ReturnStatement &stm) override;
    // TODO:
    // void visitPostfixExpression(PostfixExpression &expr) override;
    // void visitPrefixExpression(PrefixExpression &expr) override;
};
} // namespace Ceres::Typing

#endif // COMPILER_TYPECHECKVISITOR_H
