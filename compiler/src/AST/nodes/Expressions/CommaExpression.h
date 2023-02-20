#ifndef COMPILER_COMMAEXPRESSION_H
#define COMPILER_COMMAEXPRESSION_H

#include "Expression.h"
#include <memory>
#include <vector>

namespace Ceres::AST {

class CommaExpression : public Expression {
public:
    std::vector<std::unique_ptr<Expression>> expressions;

    CommaExpression(SourceSpan const& sourceSpan, std::vector<std::unique_ptr<Expression>>&& expressions);

    void accept(AbstractASTVisitor& visitor) override;

    std::vector<Node*> getChildren() const override;
};

} // namespace Ceres::AST

#endif // COMPILER_COMMAEXPRESSION_H
