#include "CodegenVisitor.h"
#include "../utils/SourceManager.h"
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
    ASSERT(def.parentFunction == nullptr);

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
    for (auto& arg : def.parameters) {
        llvm::AllocaInst* paramAlloca = builder->CreateAlloca(arg.type->getLLVMType(), nullptr, arg.id);
        arg.llvmAlloca = paramAlloca;
    }

    auto* oldCurrentFunction = currentFunction;
    currentFunction = function;
    visitChildren(def);
    currentFunction = oldCurrentFunction;

    if (!llvm::verifyFunction(*function)) {
        Log::panic("Function verification error when verifying function '{}'", def.id);
    }

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

llvm::Value* CodegenVisitor::doVisitIntLiteralExpression(AST::IntLiteralExpression& expr)
{
    PrimitiveIntegerType* type = llvm::dyn_cast<PrimitiveIntegerType>(expr.type);
    ASSERT(type != nullptr);

    // TODO: We should check in a previous pass that the literal indeed fits in the integer type
    return llvm::ConstantInt::get(type->getLLVMType(), expr.getLLVMAPInt());
}

llvm::Value* CodegenVisitor::doVisitPostfixExpression(AST::PostfixExpression& expr) { NOT_IMPLEMENTED(); }

llvm::Value* CodegenVisitor::doVisitPrefixExpression(AST::PrefixExpression& expr) { NOT_IMPLEMENTED(); }

llvm::Value* CodegenVisitor::doVisitVariableDeclaration(AST::VariableDeclaration& decl)
{
    // TODO: Handle global vs local variable declaration
    switch (decl.scope) {
    case AST::VariableScope::Local: {
        // Create an alloca instantiation at the beginning of the function block
        llvm::IRBuilder<> tmpBuilder(&currentFunction->getEntryBlock(), currentFunction->getEntryBlock().begin());
        llvm::AllocaInst* allocaInst = tmpBuilder.CreateAlloca(decl.type->getLLVMType(), nullptr, decl.id);
        decl.allocaInst = allocaInst;

        if (decl.initializerExpression != nullptr) {
            llvm::Value* initializationValue = visit(*decl.initializerExpression);
            builder->CreateStore(initializationValue, allocaInst);
        }
        return nullptr;
    }
    case AST::VariableScope::Global:
        NOT_IMPLEMENTED();
        break;
    default:
        NOT_IMPLEMENTED();
    }
}

} // Codegen