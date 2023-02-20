#ifndef COMPILER_ABSTRACTASTVISITOR_H
#define COMPILER_ABSTRACTASTVISITOR_H

#include "nodes/CompilationUnit.h"
#include "nodes/Expressions/AssignmentExpression.h"
#include "nodes/Expressions/BoolLiteralExpression.h"
#include "nodes/Expressions/CommaExpression.h"
#include "nodes/Expressions/FloatLiteralExpression.h"
#include "nodes/Expressions/FunctionCallExpression.h"
#include "nodes/Expressions/IdentifierExpression.h"
#include "nodes/Expressions/IntLiteralExpression.h"
#include "nodes/Expressions/PostfixExpression.h"
#include "nodes/Expressions/PrefixExpression.h"
#include "nodes/Node.h"
#include "nodes/Statements/BlockStatement.h"
#include "nodes/Statements/ExpressionStatement.h"
#include "nodes/Statements/ForStatement.h"
#include "nodes/Statements/IfStatement.h"
#include "nodes/Statements/ReturnStatement.h"
#include "nodes/Statements/VariableDeclarationStatement.h"
#include "nodes/Statements/WhileStatement.h"

namespace Ceres::AST {

class AbstractASTVisitor {
public:
    // This function should not be overridden, automatically calls the children
    void visitChildren(Node& node);
    void visit(Node& node);

    virtual void visitAssignmentExpression(AssignmentExpression& expr) = 0;
    virtual void visitBinaryOperationExpression(BinaryOperationExpression& expr) = 0;
    virtual void visitBlockStatement(BlockStatement& stm) = 0;
    virtual void visitBoolLiteral(BoolLiteralExpression& lit) = 0;
    virtual void visitCommaExpression(CommaExpression& expr) = 0;
    virtual void visitCompilationUnit(CompilationUnit& unit) = 0;
    virtual void visitExpressionStatement(ExpressionStatement& stm) = 0;
    virtual void visitFloatLiteralExpression(FloatLiteralExpression& expr) = 0;
    virtual void visitForStatement(ForStatement& stm) = 0;
    virtual void visitFunctionCallExpression(FunctionCallExpression& expr) = 0;
    virtual void visitFunctionDefinition(FunctionDefinition& def) = 0;
    virtual void visitIdentifierExpression(IdentifierExpression& expr) = 0;
    virtual void visitIfStatement(IfStatement& stm) = 0;
    virtual void visitIntLiteralExpression(IntLiteralExpression& expr) = 0;
    virtual void visitPostfixExpression(PostfixExpression& expr) = 0;
    virtual void visitPrefixExpression(PrefixExpression& expr) = 0;
    virtual void visitReturnStatement(ReturnStatement& stm) = 0;
    virtual void visitVariableDeclaration(VariableDeclaration& decl) = 0;
    virtual void visitVariableDeclarationStatement(VariableDeclarationStatement& stm) = 0;
    virtual void visitWhileStatement(WhileStatement& stm) = 0;
};

} // namespace Ceres::AST

#endif // COMPILER_ABSTRACTASTVISITOR_H
