#include "ASTVisitor.h"
#include "../utils/log.hpp"
#include "nodes/Statements/BlockStatement.h"

namespace Ceres::AST {

void ASTVisitor::visitAssignmentExpression(AssignmentExpression& expr) { visitChildren(expr); }

void ASTVisitor::visitBinaryOperationExpression(BinaryOperationExpression& expr) { visitChildren(expr); }

void ASTVisitor::visitBlockStatement(BlockStatement& stm) { visitChildren(stm); }

void ASTVisitor::visitBoolLiteral(BoolLiteralExpression& lit) { visitChildren(lit); }

void ASTVisitor::visitCommaExpression(CommaExpression& expr) { visitChildren(expr); }

void ASTVisitor::visitCompilationUnit(CompilationUnit& unit) { visitChildren(unit); }

void ASTVisitor::visitExpressionStatement(ExpressionStatement& stm) { visitChildren(stm); }

void ASTVisitor::visitFloatLiteralExpression(FloatLiteralExpression& expr) { visitChildren(expr); }

void ASTVisitor::visitForStatement(ForStatement& stm) { visitChildren(stm); }

void ASTVisitor::visitFunctionCallExpression(FunctionCallExpression& expr) { visitChildren(expr); }

void ASTVisitor::visitFunctionDefinition(FunctionDefinition& def) { visitChildren(def); }

void ASTVisitor::visitIdentifierExpression(IdentifierExpression& expr) { visitChildren(expr); }

void ASTVisitor::visitIfStatement(IfStatement& stm) { visitChildren(stm); }

void ASTVisitor::visitIntLiteralExpression(IntLiteralExpression& expr)
{
    // TODO: Maybe avoid visiting children for literal? They do not have any, so
    // it could be an optimization
    visitChildren(expr);
}

void ASTVisitor::visitPostfixExpression(PostfixExpression& expr) { visitChildren(expr); }

void ASTVisitor::visitPrefixExpression(PrefixExpression& expr) { visitChildren(expr); }

void ASTVisitor::visitReturnStatement(ReturnStatement& stm) { visitChildren(stm); }

void ASTVisitor::visitVariableDeclaration(VariableDeclaration& decl) { visitChildren(decl); }

void ASTVisitor::visitWhileStatement(WhileStatement& stm) { visitChildren(stm); }

void ASTVisitor::visitFunctionDeclaration(FunctionDeclaration& dec) { visitChildren(dec); }
} // namespace Ceres::AST