#ifndef COMPILER_BINARYOPERATIONEXPRESSION_H
#define COMPILER_BINARYOPERATIONEXPRESSION_H

#include "../../../Typing/BinaryOperation.h"
#include "Expression.h"
#include <memory>

namespace Ceres::AST {

class BinaryOperationExpression : public Expression {
public:
    std::unique_ptr<Expression> left, right;
    Typing::BinaryOperation op;

    SourceSpan opSpan;

    BinaryOperationExpression(SourceSpan const& sourceSpan, std::unique_ptr<Expression>&& left,
        std::unique_ptr<Expression>&& right, Typing::BinaryOperation op, SourceSpan opSpan);

    void accept(AbstractASTVisitor& visitor) override;

    std::vector<Node*> getChildren() const override;
};

} // namespace Ceres::AST

#endif // COMPILER_BINARYOPERATIONEXPRESSION_H
