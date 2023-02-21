#include "FunctionCallExpression.h"

#include "../../AbstractASTVisitor.h"
#include "IdentifierExpression.h"
#include <utility>

namespace Ceres::AST {
FunctionCallExpression::FunctionCallExpression(SourceSpan const& sourceSpan, std::string functionIdentifier,
    std::vector<std::unique_ptr<Expression>>&& arguments, SourceSpan functionIdentifierSourceSpan)
    : Expression(sourceSpan)
    , identifier(new IdentifierExpression(functionIdentifierSourceSpan, std::move(functionIdentifier)))
    , arguments(std::move(arguments))
{
}

void FunctionCallExpression::accept(AbstractASTVisitor& visitor) { visitor.visitFunctionCallExpression(*this); }

std::vector<Node*> FunctionCallExpression::getChildren() const
{
    // TODO: In the future, when we add support for function calls to pointers,
    // we need to add the expression
    //          resolving to the function pointer to the children
    std::vector<Node*> v;
    v.reserve(arguments.size() + 1);
    v.push_back(identifier.get());
    for (auto& a : arguments) {
        v.push_back(a.get());
    }
    return v;
}
} // namespace Ceres::AST