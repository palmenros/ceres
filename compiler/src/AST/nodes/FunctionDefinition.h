#ifndef COMPILER_FUNCTIONDEFINITION_H
#define COMPILER_FUNCTIONDEFINITION_H

#include "../../Typing/Type.h"
#include "../FunctionParameter.h"
#include "Node.h"
#include "Statements/BlockStatement.h"
#include <memory>
#include <string>
#include <vector>

namespace Ceres::AST {

enum class FunctionVisibility { Public, Private };

class FunctionDefinition : public Node {
public:
    FunctionVisibility visibility;
    std::string functionName;
    std::vector<FunctionParameter> parameters;
    Type* returnType;
    std::unique_ptr<BlockStatement> block;

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
