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
        arg.llvmAlloca = allocateLocalVariable(arg.type, arg.id, builder.get());
    }

    auto* oldCurrentFunction = currentFunction;
    currentFunction = function;
    visitChildren(def);
    currentFunction = oldCurrentFunction;

    if (basicBlock->getTerminator() == nullptr) {
        ASSERT(def.returnType == VoidType::get());
        builder->CreateRetVoid();
    }

    if (llvm::verifyFunction(*function, &llvm::errs())) {
        Log::panic("Function verification error when verifying function '{}'", def.id);
    }

    return function;
}

llvm::Value* CodegenVisitor::doVisitFunctionDeclaration(AST::FunctionDeclaration& def) { NOT_IMPLEMENTED(); }

/* Statements */

llvm::Value* CodegenVisitor::doVisitBlockStatement(AST::BlockStatement& stm)
{
    // TODO: Should block statement create a BasicBlock? For now we won't

    for (auto& child : stm.statements) {
        visit(*child);
        // Stop generating instructions if we have reached a terminator (return, break, etc).
        // LLVM will complain otherwise
        // TODO: Check if this is enough
        if (builder->GetInsertBlock()->getTerminator() != nullptr) {
            break;
        }
    }

    return nullptr;
}

llvm::Value* CodegenVisitor::doVisitExpressionStatement(AST::ExpressionStatement& stm)
{
    visitChildren(stm);
    return nullptr;
}

llvm::Value* CodegenVisitor::doVisitForStatement(AST::ForStatement& stm) { NOT_IMPLEMENTED(); }

llvm::Value* CodegenVisitor::doVisitIfStatement(AST::IfStatement& stm) { NOT_IMPLEMENTED(); }

llvm::Value* CodegenVisitor::doVisitReturnStatement(AST::ReturnStatement& stm)
{
    if (stm.expr != nullptr) {
        auto* value = visit(*stm.expr);
        ASSERT(value != nullptr);
        builder->CreateRet(value);
    } else {
        builder->CreateRetVoid();
    }

    return nullptr;
}

llvm::Value* CodegenVisitor::doVisitWhileStatement(AST::WhileStatement& stm) { NOT_IMPLEMENTED(); }

/* Expressions */

llvm::Value* CodegenVisitor::doVisitAssignmentExpression(AST::AssignmentExpression& expr) { NOT_IMPLEMENTED(); }

llvm::Value* CodegenVisitor::doVisitBinaryOperationExpression(AST::BinaryOperationExpression& expr)
{
    NOT_IMPLEMENTED();
}

llvm::Value* CodegenVisitor::doVisitBoolLiteral(AST::BoolLiteralExpression& lit)
{
    // TODO: Add support for if shorthand
    uint64_t val;
    switch (lit.value) {
    case AST::BoolLiteralValue::True:
        val = 1;
        break;
    case AST::BoolLiteralValue::False:
        val = 0;
        break;
    }
    return llvm::ConstantInt::get(llvm::Type::getInt1Ty(*context), llvm::APInt(1, val, false));
}

llvm::Value* CodegenVisitor::doVisitCastExpression(AST::CastExpression& expr) { NOT_IMPLEMENTED(); }

llvm::Value* CodegenVisitor::doVisitCommaExpression(AST::CommaExpression& expr) { NOT_IMPLEMENTED(); }

llvm::Value* CodegenVisitor::doVisitFloatLiteralExpression(AST::FloatLiteralExpression& expr)
{
    PrimitiveFloatType* type = llvm::dyn_cast<PrimitiveFloatType>(expr.type);
    ASSERT(type != nullptr);

    return llvm::ConstantFP::get(type->getLLVMType(), expr.getLLVMAPFloat());
}

llvm::Value* CodegenVisitor::doVisitFunctionCallExpression(AST::FunctionCallExpression& expr)
{
    // TODO: Check if this works well with both function definitions, declarations and function pointers
    llvm::Value* callee = visit(*expr.identifier);

    std::vector<llvm::Value*> args;
    args.reserve(expr.arguments.size());

    for (auto const& arg : expr.arguments) {
        args.push_back(visit(*arg));
    }

    llvm::FunctionType* functionType = llvm::dyn_cast<llvm::FunctionType>(expr.identifier->type->getLLVMType());
    ASSERT(functionType != nullptr);

    return builder->CreateCall(functionType, callee, args);
}

llvm::Value* CodegenVisitor::doVisitIdentifierExpression(AST::IdentifierExpression& expr)
{
    ASSERT(expr.decl.has_value());

    auto createLoad = [&](Type* type, llvm::Value* value, std::string const& name) {
        llvm::Type* llvmType = type->getLLVMType();
        // Note: If we are trying to load a function pointer, we have to take a pointer
        if (auto* functionType = llvm::dyn_cast<FunctionType>(type)) {
            llvmType = llvmType->getPointerTo();
        }
        return builder->CreateLoad(llvmType, value, name);
    };

    switch (expr.decl->getKind()) {
    case Binding::SymbolDeclarationKind::FunctionDefinition: {
        auto* funDef = expr.decl->getFunDef();
        return funDef->llvmFunction;
    }
    case Binding::SymbolDeclarationKind::FunctionDeclaration:
        NOT_IMPLEMENTED();
    case Binding::SymbolDeclarationKind::LocalVariableDeclaration: {
        // TODO: Check if we are in an LHS context and return a pointer instead
        auto* varDec = expr.decl->getVarDecl();
        return createLoad(varDec->type, varDec->allocaInst, varDec->id);
    }
    case Binding::SymbolDeclarationKind::GlobalVariableDeclaration:
        NOT_IMPLEMENTED();
    case Binding::SymbolDeclarationKind::FunctionParamDeclaration: {
        // TODO: Check if we are in an LHS context and return a pointer instead
        auto* funParam = expr.decl->getParam();
        return createLoad(funParam->type, funParam->llvmAlloca, funParam->id);
    }
    case Binding::SymbolDeclarationKind::Invalid:
        NOT_IMPLEMENTED();
    }
}

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
        decl.allocaInst = allocateLocalVariable(decl.type, decl.id);

        if (decl.initializerExpression != nullptr) {
            llvm::Value* initializationValue = visit(*decl.initializerExpression);
            builder->CreateStore(initializationValue, decl.allocaInst);
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

llvm::AllocaInst* CodegenVisitor::allocateLocalVariable(
    Type* type, std::string const& name, llvm::IRBuilder<>* builderToUse)
{
    llvm::Type* llvmType = type->getLLVMType();

    if (auto* functionType = llvm::dyn_cast<FunctionType>(type)) {
        // TODO: How should we handle allocas of function pointers?

        // If we are a function type, we need to create a pointer to function
        llvmType = llvmType->getPointerTo();
    }

    auto createAlloca = [&](llvm::IRBuilder<>* bld) { return bld->CreateAlloca(llvmType, nullptr, name); };

    if (builderToUse == nullptr) {
        llvm::IRBuilder<> tmpBuilder(&currentFunction->getEntryBlock(), currentFunction->getEntryBlock().begin());
        return createAlloca(&tmpBuilder);
    }
    return createAlloca(builderToUse);
}

} // Codegen