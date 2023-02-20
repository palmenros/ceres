#ifndef COMPILER_RETURNINGASTVISITOR_HPP
#define COMPILER_RETURNINGASTVISITOR_HPP

#include "../utils/log.hpp"
#include "AbstractReturningASTVisitor.hpp"

namespace Ceres::AST {

// T is the return type of the accept and each visit. T must be non void
template<class T> class ReturningASTVisitor : public AbstractReturningASTVisitor<T> {
public:
    // Default implementation for visiting children

    virtual T doVisitAssignmentExpression(AssignmentExpression& expr) override;
    virtual T doVisitBinaryOperationExpression(BinaryOperationExpression& expr) override;
    virtual T doVisitBlockStatement(BlockStatement& stm) override;
    virtual T doVisitBoolLiteral(BoolLiteralExpression& lit) override;
    virtual T doVisitCommaExpression(CommaExpression& expr) override;
    virtual T doVisitCompilationUnit(CompilationUnit& unit) override;
    virtual T doVisitExpressionStatement(ExpressionStatement& stm) override;
    virtual T doVisitFloatLiteralExpression(FloatLiteralExpression& expr) override;
    virtual T doVisitForStatement(ForStatement& stm) override;
    virtual T doVisitFunctionCallExpression(FunctionCallExpression& expr) override;
    virtual T doVisitFunctionDefinition(FunctionDefinition& def) override;
    virtual T doVisitIdentifierExpression(IdentifierExpression& expr) override;
    virtual T doVisitIfStatement(IfStatement& stm) override;
    virtual T doVisitIntLiteralExpression(IntLiteralExpression& expr) override;
    virtual T doVisitPostfixExpression(PostfixExpression& expr) override;
    virtual T doVisitPrefixExpression(PrefixExpression& expr) override;
    virtual T doVisitReturnStatement(ReturnStatement& stm) override;
    virtual T doVisitVariableDeclaration(VariableDeclaration& decl) override;
    virtual T doVisitVariableDeclarationStatement(VariableDeclarationStatement& stm) override;
    virtual T doVisitWhileStatement(WhileStatement& stm) override;
};

// Visit all children by default

template<class T> T ReturningASTVisitor<T>::doVisitAssignmentExpression(AssignmentExpression& expr)
{
    return this->visitChildren(expr);
}

template<class T> T ReturningASTVisitor<T>::doVisitBinaryOperationExpression(BinaryOperationExpression& expr)
{
    return this->visitChildren(expr);
}

template<class T> T ReturningASTVisitor<T>::doVisitBlockStatement(BlockStatement& stm)
{
    return this->visitChildren(stm);
}

template<class T> T ReturningASTVisitor<T>::doVisitBoolLiteral(BoolLiteralExpression& lit)
{
    return this->visitChildren(lit);
}

template<class T> T ReturningASTVisitor<T>::doVisitCommaExpression(CommaExpression& expr)
{
    return this->visitChildren(expr);
}

template<class T> T ReturningASTVisitor<T>::doVisitCompilationUnit(CompilationUnit& unit)
{
    return this->visitChildren(unit);
}

template<class T> T ReturningASTVisitor<T>::doVisitExpressionStatement(ExpressionStatement& stm)
{
    return this->visitChildren(stm);
}

template<class T> T ReturningASTVisitor<T>::doVisitFloatLiteralExpression(FloatLiteralExpression& expr)
{
    return this->visitChildren(expr);
}

template<class T> T ReturningASTVisitor<T>::doVisitForStatement(ForStatement& stm) { return this->visitChildren(stm); }

template<class T> T ReturningASTVisitor<T>::doVisitFunctionCallExpression(FunctionCallExpression& expr)
{
    return this->visitChildren(expr);
}

template<class T> T ReturningASTVisitor<T>::doVisitFunctionDefinition(FunctionDefinition& def)
{
    return this->visitChildren(def);
}

template<class T> T ReturningASTVisitor<T>::doVisitIdentifierExpression(IdentifierExpression& expr)
{
    return this->visitChildren(expr);
}

template<class T> T ReturningASTVisitor<T>::doVisitIfStatement(IfStatement& stm) { return this->visitChildren(stm); }

template<class T> T ReturningASTVisitor<T>::doVisitIntLiteralExpression(IntLiteralExpression& expr)
{
    return this->visitChildren(expr);
}

template<class T> T ReturningASTVisitor<T>::doVisitPostfixExpression(PostfixExpression& expr)
{
    return this->visitChildren(expr);
}

template<class T> T ReturningASTVisitor<T>::doVisitPrefixExpression(PrefixExpression& expr)
{
    return this->visitChildren(expr);
}

template<class T> T ReturningASTVisitor<T>::doVisitReturnStatement(ReturnStatement& stm)
{
    return this->visitChildren(stm);
}

template<class T> T ReturningASTVisitor<T>::doVisitVariableDeclaration(VariableDeclaration& decl)
{
    return this->visitChildren(decl);
}

template<class T> T ReturningASTVisitor<T>::doVisitVariableDeclarationStatement(VariableDeclarationStatement& stm)
{
    return this->visitChildren(stm);
}

template<class T> T ReturningASTVisitor<T>::doVisitWhileStatement(WhileStatement& stm)
{
    return this->visitChildren(stm);
}

} // namespace Ceres::AST

#endif // COMPILER_RETURNINGASTVISITOR_HPP
