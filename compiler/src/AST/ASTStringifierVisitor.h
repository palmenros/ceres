#ifndef COMPILER_ASTSTRINGIFIERVISITOR_H
#define COMPILER_ASTSTRINGIFIERVISITOR_H

#include "AbstractReturningASTVisitor.hpp"
#include <string>

namespace Ceres::AST {

class ASTStringifierVisitor : public AbstractReturningASTVisitor<std::string> {

    std::string doVisitAssignmentExpression(AssignmentExpression& expr) override;
    std::string doVisitBinaryOperationExpression(BinaryOperationExpression& expr) override;
    std::string doVisitBlockStatement(BlockStatement& stm) override;
    std::string doVisitBoolLiteral(BoolLiteralExpression& lit) override;
    std::string doVisitCommaExpression(CommaExpression& expr) override;
    std::string doVisitCompilationUnit(CompilationUnit& unit) override;
    std::string doVisitExpressionStatement(ExpressionStatement& stm) override;
    std::string doVisitFloatLiteralExpression(FloatLiteralExpression& expr) override;
    std::string doVisitForStatement(ForStatement& stm) override;
    std::string doVisitFunctionCallExpression(FunctionCallExpression& expr) override;
    std::string doVisitFunctionDefinition(FunctionDefinition& def) override;
    std::string doVisitFunctionDeclaration(FunctionDeclaration& def) override;

private:
    std::string doVisitIdentifierExpression(IdentifierExpression& expr) override;
    std::string doVisitIfStatement(IfStatement& stm) override;
    std::string doVisitIntLiteralExpression(IntLiteralExpression& expr) override;
    std::string doVisitPostfixExpression(PostfixExpression& expr) override;
    std::string doVisitPrefixExpression(PrefixExpression& expr) override;
    std::string doVisitReturnStatement(ReturnStatement& stm) override;
    std::string doVisitVariableDeclaration(VariableDeclaration& decl) override;
    std::string doVisitWhileStatement(WhileStatement& stm) override;
};

} // namespace Ceres::AST

#endif // COMPILER_ASTSTRINGIFIERVISITOR_H
