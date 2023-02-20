#include "FunctionDefinition.h"

#include "../AbstractASTVisitor.h"
#include <utility>

namespace Ceres::AST {
FunctionDefinition::FunctionDefinition(SourceSpan const& sourceSpan, FunctionVisibility visibility,
    std::string functionName, std::vector<FunctionParameter>&& parameters, Type* returnType,
    std::unique_ptr<BlockStatement>&& block, SourceSpan const& returnTypeSpan, SourceSpan const& functionNameSpan)
    : Node(sourceSpan)
    , visibility(visibility)
    , functionName(std::move(functionName))
    , parameters(std::move(parameters))
    , returnType(std::move(returnType))
    , block(std::move(block))
    , returnTypeSpan(returnTypeSpan)
    , functionNameSpan(functionNameSpan)
{
}

void FunctionDefinition::accept(AbstractASTVisitor& visitor) { visitor.visitFunctionDefinition(*this); }

std::vector<Node*> FunctionDefinition::getChildren() const { return { block.get() }; }
} // namespace Ceres::AST