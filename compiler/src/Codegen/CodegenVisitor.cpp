#include "CodegenVisitor.h"
#include "llvm/IR/Verifier.h"

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

llvm::Value* CodegenVisitor::doVisitFunctionDefinition(AST::FunctionDefinition& def)
{
    // Generate the function
    llvm::FunctionType* functionType = llvm::cast<llvm::FunctionType>(def.functionType->getLLVMType());

    // TODO: For now, all functions will be defined as external linkage
    // TODO: Add function name mangling
    llvm::Function* function
        = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, def.id, module.get());

    // Set readable name for all variables in prototype
    unsigned int index = 0;
    for (auto& arg : function->args()) {
        arg.setName(def.parameters[index].id);
        index++;
    }

    def.llvmFunction = function;

    llvm::BasicBlock* basicBlock = llvm::BasicBlock::Create(*context, "entry", function);
    builder->SetInsertPoint(basicBlock);

    // TODO: Insert parameters to stack and save them on the function

    visitChildren(def);

    llvm::verifyFunction(*function);

    return function;
}

llvm::Value* CodegenVisitor::doVisitFunctionDeclaration(AST::FunctionDeclaration& def) { NOT_IMPLEMENTED(); }

/* Statements */

llvm::Value* CodegenVisitor::doVisitBlockStatement(AST::BlockStatement& stm)
{
    // TODO: Should block statement create a BasicBlock? For now we won't
    visitChildren(stm);
    return nullptr;
}

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