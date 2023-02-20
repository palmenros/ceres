#include "FunctionDeclaration.h"
#include "../AbstractASTVisitor.h"

namespace Ceres::AST {

FunctionDeclaration::FunctionDeclaration(SourceSpan const& sourceSpan, FunctionVisibility visibility,
    std::string functionName, std::vector<FunctionParameter>&& parameters, Type* returnType,
    SourceSpan const& returnTypeSpan, SourceSpan const& functionNameSpan)
    : Node(sourceSpan)
    , visibility(visibility)
    , functionName(std::move(functionName))
    , parameters(std::move(parameters))
    , returnType(returnType)
    , returnTypeSpan(returnTypeSpan)
    , functionNameSpan(functionNameSpan)
{
}

std::vector<Node*> FunctionDeclaration::getChildren() const { return {}; }

void FunctionDeclaration::accept(AbstractASTVisitor& visitor) { visitor.visitFunctionDeclaration(*this); }

} // AST