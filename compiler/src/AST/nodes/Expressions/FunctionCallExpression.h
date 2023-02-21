#ifndef COMPILER_FUNCTIONCALLEXPRESSION_H
#define COMPILER_FUNCTIONCALLEXPRESSION_H

#include "../../../Binding/SymbolDeclaration.h"
#include "Expression.h"
#include "IdentifierExpression.h"
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace Ceres::AST {

class FunctionCallExpression : public Expression {
public:
    // TODO: maybe it can be something else
    // std::string functionIdentifier;
    std::vector<std::unique_ptr<Expression>> arguments;
    std::unique_ptr<IdentifierExpression> identifier;

    FunctionCallExpression(SourceSpan const& sourceSpan, std::string functionIdentifier,
        std::vector<std::unique_ptr<Expression>>&& arguments, SourceSpan functionIdentifierSourceSpan);

    void accept(AbstractASTVisitor& visitor) override;

    std::vector<Node*> getChildren() const override;
};

} // namespace Ceres::AST

#endif // COMPILER_FUNCTIONCALLEXPRESSION_H
