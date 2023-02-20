#ifndef COMPILER_FUNCTIONCALLEXPRESSION_H
#define COMPILER_FUNCTIONCALLEXPRESSION_H

#include "Expression.h"
#include <memory>
#include <string>
#include <vector>

namespace Ceres::AST {

class FunctionCallExpression : public Expression {
public:
    std::string functionIdentifier;
    std::vector<std::unique_ptr<Expression>> arguments;

    SourceSpan functionIdentifierSourceSpan;

    FunctionCallExpression(SourceSpan const& sourceSpan, std::string functionIdentifier,
        std::vector<std::unique_ptr<Expression>>&& arguments, SourceSpan functionIdentifierSourceSpan);

    void accept(AbstractASTVisitor& visitor) override;

    std::vector<Node*> getChildren() const override;
};

} // namespace Ceres::AST

#endif // COMPILER_FUNCTIONCALLEXPRESSION_H
