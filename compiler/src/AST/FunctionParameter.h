#ifndef COMPILER_FUNCTIONPARAMETER_H
#define COMPILER_FUNCTIONPARAMETER_H

#include "../Typing/Type.h"
#include "nodes/Statements/VariableDeclaration.h"
#include <memory>

namespace Ceres::AST {

struct FunctionParameter {
    Type* type;
    std::string name;
    VariableConstness constness;

    SourceSpan typeSourceSpan;
    SourceSpan parameterNameSourceSpan;

    FunctionParameter(Type* type, std::string name, VariableConstness constness, SourceSpan typeSourceSpan,
        SourceSpan parameterNameSourceSpan);
};

} // namespace Ceres::AST

#endif // COMPILER_FUNCTIONPARAMETER_H
