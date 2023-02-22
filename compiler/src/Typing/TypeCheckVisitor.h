#ifndef COMPILER_TYPECHECKVISITOR_H
#define COMPILER_TYPECHECKVISITOR_H

#include "../AST/ASTVisitor.h"
#include "../Binding/Scope.h"

namespace Ceres::Typing {

class TypeCheckVisitor : public AST::ASTVisitor {
public:
    // Type check
    void visitVariableDeclaration(AST::VariableDeclaration& decl) override;
    void visitAssignmentExpression(AST::AssignmentExpression& expr) override;
    void visitBinaryOperationExpression(AST::BinaryOperationExpression& expr) override;
    // TODO:
    // void visitCommaExpression(CommaExpression &expr) override;
    void visitFunctionCallExpression(AST::FunctionCallExpression& expr) override;
    void visitIdentifierExpression(AST::IdentifierExpression& expr) override;
    // TODO:
    // void visitPostfixExpression(PostfixExpression &expr) override;
    // void visitPrefixExpression(PrefixExpression &expr) override;

    // Statements
    void visitReturnStatement(AST::ReturnStatement& stm) override;
    void visitIfStatement(AST::IfStatement& stm) override;
    void visitCommaExpression(AST::CommaExpression& expr) override;
    void visitForStatement(AST::ForStatement& stm) override;
    void visitWhileStatement(AST::WhileStatement& stm) override;
    void visitPostfixExpression(AST::PostfixExpression& expr) override;
    void visitPrefixExpression(AST::PrefixExpression& expr) override;
};
} // namespace Ceres::Typing

#endif // COMPILER_TYPECHECKVISITOR_H
