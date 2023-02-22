#ifndef COMPILER_ASTVISITOR_H
#define COMPILER_ASTVISITOR_H

#include "AbstractASTVisitor.h"
#include "nodes/Statements/BlockStatement.h"

namespace Ceres::AST {

class ASTVisitor : public AbstractASTVisitor {
public:
    // These functions should be overriden to do the required functionality
    // Expressions
    virtual void visitAssignmentExpression(AssignmentExpression& expr) override;
    virtual void visitBinaryOperationExpression(BinaryOperationExpression& expr) override;
    virtual void visitCommaExpression(CommaExpression& expr) override;
    virtual void visitFloatLiteralExpression(FloatLiteralExpression& expr) override;
    virtual void visitFunctionCallExpression(FunctionCallExpression& expr) override;
    virtual void visitIdentifierExpression(IdentifierExpression& expr) override;
    virtual void visitIntLiteralExpression(IntLiteralExpression& expr) override;
    virtual void visitPostfixExpression(PostfixExpression& expr) override;
    virtual void visitPrefixExpression(PrefixExpression& expr) override;
    virtual void visitBoolLiteral(BoolLiteralExpression& lit) override;
    virtual void visitFunctionDeclaration(FunctionDeclaration& dec) override;
    virtual void visitCastExpression(CastExpression& expr) override;

    // Statements
    virtual void visitExpressionStatement(ExpressionStatement& stm) override;
    virtual void visitVariableDeclaration(VariableDeclaration& decl) override;
    virtual void visitCompilationUnit(CompilationUnit& unit) override;
    virtual void visitBlockStatement(BlockStatement& stm) override;
    virtual void visitForStatement(ForStatement& stm) override;
    virtual void visitIfStatement(IfStatement& stm) override;
    virtual void visitReturnStatement(ReturnStatement& stm) override;
    virtual void visitWhileStatement(WhileStatement& stm) override;
    virtual void visitFunctionDefinition(FunctionDefinition& def) override;
};

} // namespace Ceres::AST

#endif // COMPILER_ASTVISITOR_H
