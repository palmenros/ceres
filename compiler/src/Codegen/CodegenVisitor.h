#ifndef COMPILER_CODEGENVISITOR_H
#define COMPILER_CODEGENVISITOR_H

#include "../AST/AbstractReturningASTVisitor.hpp"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>

namespace Ceres::Codegen {

class CodegenVisitor : public AST::AbstractReturningASTVisitor<llvm::Value*> {

    llvm::LLVMContext* context;
    std::unique_ptr<llvm::IRBuilder<>> builder;
    std::unique_ptr<llvm::Module> module;
    llvm::Function* currentFunction = nullptr;

    llvm::AllocaInst* allocateLocalVariable(
        Type* type, std::string const& name, llvm::IRBuilder<>* builderToUse = nullptr);

public:
    CodegenVisitor(llvm::LLVMContext* context);

public:
    llvm::Value* doVisitAssignmentExpression(AST::AssignmentExpression& expr) override;
    llvm::Value* doVisitBinaryOperationExpression(AST::BinaryOperationExpression& expr) override;
    llvm::Value* doVisitBlockStatement(AST::BlockStatement& stm) override;
    llvm::Value* doVisitBoolLiteral(AST::BoolLiteralExpression& lit) override;
    llvm::Value* doVisitCastExpression(AST::CastExpression& expr) override;
    llvm::Value* doVisitCommaExpression(AST::CommaExpression& expr) override;
    llvm::Value* doVisitCompilationUnit(AST::CompilationUnit& unit) override;
    llvm::Value* doVisitExpressionStatement(AST::ExpressionStatement& stm) override;
    llvm::Value* doVisitFloatLiteralExpression(AST::FloatLiteralExpression& expr) override;
    llvm::Value* doVisitForStatement(AST::ForStatement& stm) override;
    llvm::Value* doVisitFunctionCallExpression(AST::FunctionCallExpression& expr) override;
    llvm::Value* doVisitFunctionDefinition(AST::FunctionDefinition& def) override;
    llvm::Value* doVisitIdentifierExpression(AST::IdentifierExpression& expr) override;
    llvm::Value* doVisitIfStatement(AST::IfStatement& stm) override;
    llvm::Value* doVisitIntLiteralExpression(AST::IntLiteralExpression& expr) override;
    llvm::Value* doVisitPostfixExpression(AST::PostfixExpression& expr) override;
    llvm::Value* doVisitPrefixExpression(AST::PrefixExpression& expr) override;
    llvm::Value* doVisitReturnStatement(AST::ReturnStatement& stm) override;
    llvm::Value* doVisitVariableDeclaration(AST::VariableDeclaration& decl) override;
    llvm::Value* doVisitWhileStatement(AST::WhileStatement& stm) override;
    llvm::Value* doVisitFunctionDeclaration(AST::FunctionDeclaration& def) override;

    friend class CodeGenerator;
};

} // Codegen

#endif // COMPILER_CODEGENVISITOR_H
