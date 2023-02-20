#ifndef COMPILER_TYPECHECKVISITOR_H
#define COMPILER_TYPECHECKVISITOR_H

#include "../AST/ASTVisitor.h"
#include "../Binding/Scope.h"

namespace Ceres::Typing {

class TypeCheckVisitor : public AST::ASTVisitor {
public:
    Binding::Scope* currentScope = nullptr;
    AST::FunctionDefinition* currentFunction = nullptr;

    // Reconstruct scopes
    void visitCompilationUnit(AST::CompilationUnit& unit) override;
    void visitBlockStatement(AST::BlockStatement& stm) override;
    void visitFunctionDefinition(AST::FunctionDefinition& def) override;

    // Type check
    void visitVariableDeclaration(AST::VariableDeclaration& decl) override;
    void visitAssignmentExpression(AST::AssignmentExpression& expr) override;
    void visitBinaryOperationExpression(AST::BinaryOperationExpression& expr) override;
    // TODO:
    // void visitCommaExpression(CommaExpression &expr) override;
    void visitFunctionCallExpression(AST::FunctionCallExpression& expr) override;
    void visitIdentifierExpression(AST::IdentifierExpression& expr) override;
    void visitReturnStatement(AST::ReturnStatement& stm) override;
    // TODO:
    // void visitPostfixExpression(PostfixExpression &expr) override;
    // void visitPrefixExpression(PrefixExpression &expr) override;
};
} // namespace Ceres::Typing

#endif // COMPILER_TYPECHECKVISITOR_H
