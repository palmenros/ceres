#ifndef COMPILER_FUNCTIONPARAMETER_H
#define COMPILER_FUNCTIONPARAMETER_H

#include "../Typing/Type.h"
#include "nodes/Statements/VariableDeclaration.h"
#include <llvm/IR/Instructions.h>
#include <memory>

namespace Ceres::AST {

struct FunctionParameter {
    Type* type;
    std::string id;
    Typing::Constness constness;

    SourceSpan typeSourceSpan;
    SourceSpan parameterNameSourceSpan;

    llvm::AllocaInst* llvmAlloca = nullptr;

    FunctionParameter(Type* type, std::string name, Typing::Constness constness, SourceSpan typeSourceSpan,
        SourceSpan parameterNameSourceSpan);
};

} // namespace Ceres::AST

#endif // COMPILER_FUNCTIONPARAMETER_H
