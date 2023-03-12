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

    //  First visit all function declarations and definitions (before visiting children),
    //      to setup adequate pointers.
    for (auto& def : unit.functionDefinitions) {
        generateFunctionDefinitionPrototype(*def);
    }

    for (auto& dec : unit.functionDeclarations) {
        generateFunctionDeclarationPrototype(*dec);
    }

    for (auto& globalVar : unit.globalVariableDeclarations) {
        generateGlobalVariablePrototype(*globalVar);
    }

    visitChildren(unit);
    return nullptr;
}

void CodegenVisitor::generateFunctionDefinitionPrototype(AST::FunctionDefinition& def)
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
}

void CodegenVisitor::generateFunctionDeclarationPrototype(AST::FunctionDeclaration& dec)
{
    // Generate the function
    llvm::FunctionType* functionType = llvm::cast<llvm::FunctionType>(dec.functionType->getLLVMType());

    // TODO: For now, all functions will be defined as external linkage
    // TODO: Add function name mangling
    llvm::Function* function
        = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, dec.functionName, module.get());

    // Set readable name for all variables in prototype
    unsigned int index = 0;
    for (auto& arg : function->args()) {
        arg.setName(dec.parameters[index].id);
        index++;
    }

    dec.llvmFunction = function;
}

void CodegenVisitor::generateGlobalVariablePrototype(AST::VariableDeclaration& dec) { NOT_IMPLEMENTED(); }

llvm::Value* CodegenVisitor::doVisitFunctionDefinition(AST::FunctionDefinition& def)
{
    // TODO: Add support for nested functions
    ASSERT(def.parentFunction == nullptr);
    ASSERT(def.llvmFunction != nullptr);

    auto* function = def.llvmFunction;

    llvm::BasicBlock* basicBlock = llvm::BasicBlock::Create(*context, "entry", function);
    builder->SetInsertPoint(basicBlock);

    // Create alloca for arguments
    for (auto& arg : def.parameters) {
        arg.llvmAlloca = allocateLocalVariable(arg.type, arg.id, builder.get());
    }

    // Create store for arguments
    unsigned paramIndex = 0;
    for (auto& llvmArg : def.llvmFunction->args()) {
        auto const& arg = def.parameters[paramIndex];
        generateStore(arg.llvmAlloca, &llvmArg);
        paramIndex++;
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

llvm::Value* CodegenVisitor::doVisitFunctionDeclaration(AST::FunctionDeclaration& dec)
{
    ASSERT(dec.llvmFunction != nullptr);
    return dec.llvmFunction;
}

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

llvm::Value* CodegenVisitor::doVisitForStatement(AST::ForStatement& stm) { TODO(); }

llvm::Value* CodegenVisitor::doVisitIfStatement(AST::IfStatement& stm)
{
    ASSERT(!shouldGenerateShortCircuitBooleanCode);

    auto* thenBasicBlock = llvm::BasicBlock::Create(*context, "then", currentFunction);

    // Generate elseBasicBlock first (sooner than contBasicBlock) for better LLVM-IR visualization
    auto* elseBasicBlock
        = stm.maybeElseStatement != nullptr ? llvm::BasicBlock::Create(*context, "else", currentFunction) : nullptr;

    auto* contBasicBlock = llvm::BasicBlock::Create(*context, "ifcont", currentFunction);

    if (stm.maybeElseStatement != nullptr) {
        // There is an else statement

        /* Visit conditional expression*/
        enableBooleanShortCircuit(thenBasicBlock, elseBasicBlock);
        visit(*stm.condition);
        disableBooleanShortCircuit();

        /* Visit then block */
        builder->SetInsertPoint(thenBasicBlock);
        visit(*stm.thenBlock);
        builder->CreateBr(contBasicBlock);

        /* Visit else block */
        builder->SetInsertPoint(elseBasicBlock);
        visit(*stm.maybeElseStatement);
        builder->CreateBr(contBasicBlock);

    } else {
        // There is no else

        /*Visit conditional expression */
        enableBooleanShortCircuit(thenBasicBlock, contBasicBlock);
        visit(*stm.condition);
        disableBooleanShortCircuit();

        /*Visit then block*/
        builder->SetInsertPoint(thenBasicBlock);
        visit(*stm.thenBlock);
        builder->CreateBr(contBasicBlock);
    }

    builder->SetInsertPoint(contBasicBlock);
    return nullptr;
}

void CodegenVisitor::enableBooleanShortCircuit(llvm::BasicBlock* newTrueLabel, llvm::BasicBlock* newFalseLabel)
{
    shouldGenerateShortCircuitBooleanCode = true;
    trueLabel = newTrueLabel;
    falseLabel = newFalseLabel;
}

void CodegenVisitor::disableBooleanShortCircuit()
{
    ASSERT(shouldGenerateShortCircuitBooleanCode);

    shouldGenerateShortCircuitBooleanCode = false;
    trueLabel = nullptr;
    falseLabel = nullptr;
}

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

llvm::Value* CodegenVisitor::doVisitWhileStatement(AST::WhileStatement& stm) { TODO(); }

/* Expressions */

llvm::Value* CodegenVisitor::doVisitAssignmentExpression(AST::AssignmentExpression& expr)
{
    bool oldLHSVisitingMode = LHSVisitingMode;
    LHSVisitingMode = true;

    llvm::Value* ptr = visit(*expr.expressionLHS);
    LHSVisitingMode = false;

    llvm::Value* right = visit(*expr.expressionRHS);

    llvm::Value* returnVal = nullptr;

    if (expr.binaryOp.has_value()) {
        llvm::Value* left = generateLoad(expr.type, ptr);
        llvm::Value* value = generateBinaryOperation(left, right, *expr.binaryOp, expr.type);
        // TODO: Handle volatile
        builder->CreateStore(value, ptr, false);
        returnVal = value;
    } else {
        // TODO: Handle volatile
        builder->CreateStore(right, ptr, false);
        returnVal = right;
    }

    LHSVisitingMode = oldLHSVisitingMode;
    return returnVal;
}

llvm::Value* CodegenVisitor::doVisitBinaryOperationExpression(AST::BinaryOperationExpression& expr)
{
    ASSERT(expr.left->type == expr.right->type);
    Type* type = expr.left->type;

    return generateBinaryOperation(visit(*expr.left), visit(*expr.right), expr.op, type);
}

llvm::Value* CodegenVisitor::generateBinaryOperation(
    llvm::Value* left, llvm::Value* right, Typing::BinaryOperation op, Type* type)
{
    switch (op.kind) {
    case Typing::BinaryOperation::Mult: {
        if (auto* intType = llvm::dyn_cast<PrimitiveIntegerType>(type)) {
            // TODO: Check the semantics of signed overflow (NSW) and unsigned overflow (NUW)
            return builder->CreateMul(left, right, "mul");
        } else if (auto* floatType = llvm::dyn_cast<PrimitiveFloatType>(type)) {
            return builder->CreateFMul(left, right, "fmul");
        } else {
            NOT_IMPLEMENTED();
        }
        break;
    }
    case Typing::BinaryOperation::Div: {
        // TODO: Check the semantics of division
        if (auto* intType = llvm::dyn_cast<PrimitiveIntegerType>(type)) {
            // TODO: Check the semantics of signed overflow (NSW) and unsigned overflow (NUW)
            if (intType->isSigned()) {
                return builder->CreateSDiv(left, right, "sdiv");
            } else {
                return builder->CreateUDiv(left, right, "udiv");
            }
        } else if (auto* floatType = llvm::dyn_cast<PrimitiveFloatType>(type)) {
            return builder->CreateFDiv(left, right, "fdiv");
        } else {
            NOT_IMPLEMENTED();
        }
        break;
    }
    case Typing::BinaryOperation::Modulo: {
        // TODO: Check the semantics of modulo
        if (auto* intType = llvm::dyn_cast<PrimitiveIntegerType>(type)) {
            // TODO: Check the semantics of signed overflow (NSW) and unsigned overflow (NUW)
            if (intType->isSigned()) {
                return builder->CreateSRem(left, right, "srem");
            } else {
                return builder->CreateURem(left, right, "urem");
            }
        } else if (auto* floatType = llvm::dyn_cast<PrimitiveFloatType>(type)) {
            // TODO: Check semantics of floating point modulo
            return builder->CreateFRem(left, right, "frem");
        } else {
            NOT_IMPLEMENTED();
        }
        break;
    }
    case Typing::BinaryOperation::Sum: {
        if (auto* intType = llvm::dyn_cast<PrimitiveIntegerType>(type)) {
            // TODO: Check the semantics of signed overflow (NSW) and unsigned overflow (NUW)
            return builder->CreateAdd(left, right, "add");
        } else if (auto* floatType = llvm::dyn_cast<PrimitiveFloatType>(type)) {
            return builder->CreateFAdd(left, right, "fadd");
        } else {
            NOT_IMPLEMENTED();
        }
        break;
    }
    case Typing::BinaryOperation::Subtraction: {
        if (auto* intType = llvm::dyn_cast<PrimitiveIntegerType>(type)) {
            // TODO: Check the semantics of signed overflow (NSW) and unsigned overflow (NUW)
            return builder->CreateSub(left, right, "sub");
        } else if (auto* floatType = llvm::dyn_cast<PrimitiveFloatType>(type)) {
            return builder->CreateFSub(left, right, "fsub");
        } else {
            NOT_IMPLEMENTED();
        }
        break;
    }
    // TODO: Check semantics of bitwise operations on signed-unsigned
    case Typing::BinaryOperation::BitshiftLeft:
        TODO();
        break;
    case Typing::BinaryOperation::BitshiftRight:
        TODO();
        break;
    case Typing::BinaryOperation::BitwiseAnd:
        TODO();
        break;
    case Typing::BinaryOperation::BitwiseOr:
        TODO();
        break;
    case Typing::BinaryOperation::BitwiseXor:
        TODO();
        break;
    case Typing::BinaryOperation::Equals: {
        if (auto* intType = llvm::dyn_cast<PrimitiveIntegerType>(type)) {
            return generateRelationalCmp(llvm::CmpInst::ICMP_EQ, left, right);
        } else if (auto* floatType = llvm::dyn_cast<PrimitiveFloatType>(type)) {
            // TODO: Review semantics of float comparison (oeq vs ueq)
            return generateRelationalCmp(llvm::CmpInst::FCMP_OEQ, left, right);
        } else {
            NOT_IMPLEMENTED();
        }
        break;
    }
    case Typing::BinaryOperation::NotEquals: {
        if (auto* intType = llvm::dyn_cast<PrimitiveIntegerType>(type)) {
            return generateRelationalCmp(llvm::CmpInst::ICMP_NE, left, right);
        } else if (auto* floatType = llvm::dyn_cast<PrimitiveFloatType>(type)) {
            // TODO: Review semantics of float comparison (oeq vs ueq)
            return generateRelationalCmp(llvm::CmpInst::FCMP_ONE, left, right);
        } else {
            NOT_IMPLEMENTED();
        }
        break;
    }
    case Typing::BinaryOperation::LessOrEqual: {
        if (auto* intType = llvm::dyn_cast<PrimitiveIntegerType>(type)) {
            if (intType->isSigned()) {
                return generateRelationalCmp(llvm::CmpInst::ICMP_SLE, left, right);
            } else {
                return generateRelationalCmp(llvm::CmpInst::ICMP_ULE, left, right);
            }
        } else if (auto* floatType = llvm::dyn_cast<PrimitiveFloatType>(type)) {
            // TODO: Review semantics of float comparison (oeq vs ueq)
            return generateRelationalCmp(llvm::CmpInst::FCMP_OLE, left, right);
        } else {
            NOT_IMPLEMENTED();
        }
        break;
    }
    case Typing::BinaryOperation::GreaterOrEqual: {
        if (auto* intType = llvm::dyn_cast<PrimitiveIntegerType>(type)) {
            if (intType->isSigned()) {
                return generateRelationalCmp(llvm::CmpInst::ICMP_SGE, left, right);
            } else {
                return generateRelationalCmp(llvm::CmpInst::ICMP_UGE, left, right);
            }
        } else if (auto* floatType = llvm::dyn_cast<PrimitiveFloatType>(type)) {
            // TODO: Review semantics of float comparison (oeq vs ueq)
            return generateRelationalCmp(llvm::CmpInst::FCMP_OGE, left, right);
        } else {
            NOT_IMPLEMENTED();
        }
        break;
    }
    case Typing::BinaryOperation::GreaterThan: {
        if (auto* intType = llvm::dyn_cast<PrimitiveIntegerType>(type)) {
            if (intType->isSigned()) {
                return generateRelationalCmp(llvm::CmpInst::ICMP_SGT, left, right);
            } else {
                return generateRelationalCmp(llvm::CmpInst::ICMP_UGT, left, right);
            }
        } else if (auto* floatType = llvm::dyn_cast<PrimitiveFloatType>(type)) {
            // TODO: Review semantics of float comparison (oeq vs ueq)
            return generateRelationalCmp(llvm::CmpInst::FCMP_OGT, left, right);
        } else {
            NOT_IMPLEMENTED();
        }
        break;
    }
    case Typing::BinaryOperation::LessThan: {
        if (auto* intType = llvm::dyn_cast<PrimitiveIntegerType>(type)) {
            if (intType->isSigned()) {
                return generateRelationalCmp(llvm::CmpInst::ICMP_SGT, left, right);
            } else {
                return generateRelationalCmp(llvm::CmpInst::ICMP_UGT, left, right);
            }
        } else if (auto* floatType = llvm::dyn_cast<PrimitiveFloatType>(type)) {
            // TODO: Review semantics of float comparison (oeq vs ueq)
            return generateRelationalCmp(llvm::CmpInst::FCMP_OGT, left, right);
        } else {
            NOT_IMPLEMENTED();
        }
        break;
    }
    // TODO: Add support for && and || using short-circuit
    case Typing::BinaryOperation::LogicalAnd:
        TODO();
        break;
    case Typing::BinaryOperation::LogicalOr:
        TODO();
        break;
    case Typing::BinaryOperation::Invalid:
    default:
        NOT_IMPLEMENTED();
    }
}

llvm::Value* CodegenVisitor::generateRelationalCmp(llvm::CmpInst::Predicate pred, llvm::Value* left, llvm::Value* right)
{
    llvm::Value* value = builder->CreateCmp(pred, left, right);

    if (shouldGenerateShortCircuitBooleanCode) {
        ASSERT(trueLabel != nullptr);
        ASSERT(falseLabel != nullptr);

        builder->CreateCondBr(value, trueLabel, falseLabel);
    }

    return value;
}

llvm::Value* CodegenVisitor::doVisitBoolLiteral(AST::BoolLiteralExpression& lit)
{
    bool literalValue = lit.getLiteralBool();

    uint64_t intVal = literalValue ? 1 : 0;
    llvm::Value* boolValue = llvm::ConstantInt::get(llvm::Type::getInt1Ty(*context), llvm::APInt(1, intVal, false));

    if (!shouldGenerateShortCircuitBooleanCode) {
        /* Return a normal value */
        return boolValue;
    } else {
        /* Generate a branch depending of the value */
        ASSERT(trueLabel != nullptr);
        ASSERT(falseLabel != nullptr);

        builder->CreateCondBr(boolValue, trueLabel, falseLabel);
        return boolValue;
    }
}

llvm::Value* CodegenVisitor::doVisitCastExpression(AST::CastExpression& expr)
{
    // TODO: Do not forget to handle operator && and || when casting to bool
    TODO();
}

llvm::Value* CodegenVisitor::doVisitCommaExpression(AST::CommaExpression& expr)
{
    // TODO: Check how we handle operator && and || short-circuit within comma expressions
    llvm::Value* value = nullptr;
    for (auto& e : expr.expressions) {
        value = visit(*e);
    }
    return value;
}

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
    ASSERT(callee != nullptr);

    // TODO: Handle short circuit boolean code
    ASSERT(!shouldGenerateShortCircuitBooleanCode);

    return builder->CreateCall(functionType, callee, args);
}

llvm::Value* CodegenVisitor::generateLoad(Type* type, llvm::Value* ptr, std::string const& name)
{
    llvm::Type* llvmType = type->getLLVMType();
    // Note: If we are trying to load a function pointer, we have to take a pointer
    if (auto* functionType = llvm::dyn_cast<FunctionType>(type)) {
        llvmType = llvmType->getPointerTo();
    }

    llvm::Value* loadedValue = builder->CreateLoad(llvmType, ptr, name);

    // If we are loading a boolean, we have to check for short circuit
    if (type == BoolType::get() && shouldGenerateShortCircuitBooleanCode) {
        // Generate short-circuit branches
        ASSERT(trueLabel != nullptr);
        ASSERT(falseLabel != nullptr);
        builder->CreateCondBr(loadedValue, trueLabel, falseLabel);
    }

    return loadedValue;
}

llvm::Value* CodegenVisitor::doVisitIdentifierExpression(AST::IdentifierExpression& expr)
{
    ASSERT(expr.decl.has_value());

    switch (expr.decl->getKind()) {
    case Binding::SymbolDeclarationKind::FunctionDefinition: {
        auto* funDef = expr.decl->getFunDef();
        ASSERT(funDef != nullptr);
        ASSERT(funDef->llvmFunction != nullptr);
        return funDef->llvmFunction;
    }
    case Binding::SymbolDeclarationKind::FunctionDeclaration: {
        auto* funDec = expr.decl->getFunDec();
        ASSERT(funDec != nullptr);
        ASSERT(funDec->llvmFunction != nullptr);
        return funDec->llvmFunction;
    }
    case Binding::SymbolDeclarationKind::LocalVariableDeclaration: {
        auto* varDec = expr.decl->getVarDecl();
        if (LHSVisitingMode) {
            return varDec->allocaInst;
        } else {
            return generateLoad(varDec->type, varDec->allocaInst, varDec->id);
        }
    }
    case Binding::SymbolDeclarationKind::GlobalVariableDeclaration:
        // TODO: Check if we are in an LHS context and return a pointer instead
        TODO();
    case Binding::SymbolDeclarationKind::FunctionParamDeclaration: {
        auto* funParam = expr.decl->getParam();
        if (LHSVisitingMode) {
            return funParam->llvmAlloca;
        } else {
            return generateLoad(funParam->type, funParam->llvmAlloca, funParam->id);
        }
    }
    case Binding::SymbolDeclarationKind::Invalid:
    default:
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

llvm::Value* CodegenVisitor::doVisitPostfixExpression(AST::PostfixExpression& expr) { TODO(); }

llvm::Value* CodegenVisitor::doVisitPrefixExpression(AST::PrefixExpression& expr) { TODO(); }

llvm::Value* CodegenVisitor::doVisitVariableDeclaration(AST::VariableDeclaration& decl)
{
    // TODO: Handle global vs local variable declaration
    switch (decl.scope) {
    case AST::VariableScope::Local: {
        // Create an alloca instantiation at the beginning of the function block
        decl.allocaInst = allocateLocalVariable(decl.type, decl.id);

        if (decl.initializerExpression != nullptr) {
            llvm::Value* initializationValue = visit(*decl.initializerExpression);
            generateStore(decl.allocaInst, initializationValue);
        }
        return nullptr;
    }
    case AST::VariableScope::Global:
        TODO();
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
        // TODO: Check that this is the correct way of handling allocas of function pointers
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

void CodegenVisitor::generateStore(llvm::AllocaInst* allocaInst, llvm::Value* value)
{
    builder->CreateStore(value, allocaInst);
}

} // Codegen