#include "CodegenVisitor.h"

namespace Ceres::Codegen {

CodegenVisitor::CodegenVisitor(llvm::LLVMContext* context)
    : context(context)
{
    builder = std::make_unique<llvm::IRBuilder<>>(*context);
}

llvm::Value* CodegenVisitor::doVisitCompilationUnit(AST::CompilationUnit& unit)
{
    // Create llvm::module
    // TODO: Change module name to something meaningful
    module = std::make_unique<llvm::Module>("CeresModule", *context);

    visitChildren(unit);
    return nullptr;
}

llvm::Value* CodegenVisitor::doVisitFunctionDefinition(AST::FunctionDefinition& def) {
    // Generate the function
    NOT_IMPLEMENTED();
}

llvm::Value* CodegenVisitor::doVisitFunctionDeclaration(AST::FunctionDeclaration& def) { NOT_IMPLEMENTED(); }

/* Statements */

llvm::Value* CodegenVisitor::doVisitBlockStatement(AST::BlockStatement& stm) { NOT_IMPLEMENTED(); }

llvm::Value* CodegenVisitor::doVisitExpressionStatement(AST::ExpressionStatement& stm) { NOT_IMPLEMENTED(); }

llvm::Value* CodegenVisitor::doVisitForStatement(AST::ForStatement& stm) { NOT_IMPLEMENTED(); }

llvm::Value* CodegenVisitor::doVisitIfStatement(AST::IfStatement& stm) { NOT_IMPLEMENTED(); }

llvm::Value* CodegenVisitor::doVisitReturnStatement(AST::ReturnStatement& stm) { NOT_IMPLEMENTED(); }

llvm::Value* CodegenVisitor::doVisitWhileStatement(AST::WhileStatement& stm) { NOT_IMPLEMENTED(); }

/* Expressions */

llvm::Value* CodegenVisitor::doVisitAssignmentExpression(AST::AssignmentExpression& expr) { NOT_IMPLEMENTED(); }

llvm::Value* CodegenVisitor::doVisitBinaryOperationExpression(AST::BinaryOperationExpression& expr)
{
    NOT_IMPLEMENTED();
}

llvm::Value* CodegenVisitor::doVisitBoolLiteral(AST::BoolLiteralExpression& lit) { NOT_IMPLEMENTED(); }

llvm::Value* CodegenVisitor::doVisitCastExpression(AST::CastExpression& expr) { NOT_IMPLEMENTED(); }

llvm::Value* CodegenVisitor::doVisitCommaExpression(AST::CommaExpression& expr) { NOT_IMPLEMENTED(); }

llvm::Value* CodegenVisitor::doVisitFloatLiteralExpression(AST::FloatLiteralExpression& expr) { NOT_IMPLEMENTED(); }

llvm::Value* CodegenVisitor::doVisitFunctionCallExpression(AST::FunctionCallExpression& expr) { NOT_IMPLEMENTED(); }

llvm::Value* CodegenVisitor::doVisitIdentifierExpression(AST::IdentifierExpression& expr) { NOT_IMPLEMENTED(); }

llvm::Value* CodegenVisitor::doVisitIntLiteralExpression(AST::IntLiteralExpression& expr) { NOT_IMPLEMENTED(); }

llvm::Value* CodegenVisitor::doVisitPostfixExpression(AST::PostfixExpression& expr) { NOT_IMPLEMENTED(); }

llvm::Value* CodegenVisitor::doVisitPrefixExpression(AST::PrefixExpression& expr) { NOT_IMPLEMENTED(); }

llvm::Value* CodegenVisitor::doVisitVariableDeclaration(AST::VariableDeclaration& decl)
{
    // TODO: Handle global vs local variable declaration
    NOT_IMPLEMENTED();
}

} // Codegen