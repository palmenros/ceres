#ifndef COMPILER_FUNCTIONDEFINITION_H
#define COMPILER_FUNCTIONDEFINITION_H

#include "../../../Typing/Type.h"
#include "../../FunctionParameter.h"
#include "../Expressions/Expression.h"
#include "BlockStatement.h"
#include <llvm/IR/Function.h>
#include <memory>
#include <string>
#include <vector>

namespace Ceres::AST {

enum class FunctionVisibility { Public, Private, Extern };

class FunctionDefinition : public Statement {
public:
    FunctionVisibility visibility;
    std::string id;
    std::vector<FunctionParameter> parameters;
    Type* returnType;
    std::unique_ptr<BlockStatement> block;

    Type* functionType = nullptr;

    // parentFunction will be a link to the FunctionDefinition AST node of the parent function, to support
    //  nested functions
    FunctionDefinition* parentFunction = nullptr;

    llvm::Function* llvmFunction = nullptr;

    SourceSpan returnTypeSpan;
    SourceSpan functionNameSpan;

    FunctionDefinition(SourceSpan const& sourceSpan, FunctionVisibility visibility, std::string functionName,
        std::vector<FunctionParameter>&& parameters, Type* returnType, std::unique_ptr<BlockStatement>&& block,
        SourceSpan const& returnTypeSpan, SourceSpan const& functionNameSpan);

    void accept(AbstractASTVisitor& visitor) override;

    std::vector<Node*> getChildren() const override;
};

} // namespace Ceres::AST

#endif // COMPILER_FUNCTIONDEFINITION_H
