#ifndef COMPILER_FUNCTIONDECLARATION_H
#define COMPILER_FUNCTIONDECLARATION_H

#include "Node.h"
#include "Statements/FunctionDefinition.h"
#include <string>

namespace Ceres::AST {
class FunctionDeclaration : public Node {
public:
    FunctionVisibility visibility;
    std::string functionName;
    std::vector<FunctionParameter> parameters;
    Type* returnType;

    Type* functionType = nullptr;

    SourceSpan returnTypeSpan;
    SourceSpan functionNameSpan;

    FunctionDeclaration(SourceSpan const& sourceSpan, FunctionVisibility visibility, std::string functionName,
        std::vector<FunctionParameter>&& parameters, Type* returnType, SourceSpan const& returnTypeSpan,
        SourceSpan const& functionNameSpan);

    void accept(AbstractASTVisitor& visitor) override;

    std::vector<Node*> getChildren() const override;
    llvm::Value* llvmFunction = nullptr;
};
}
#endif // COMPILER_FUNCTIONDECLARATION_H
