#ifndef COMPILER_BINARYOPERATIONEXPRESSION_H
#define COMPILER_BINARYOPERATIONEXPRESSION_H

#include "Expression.h"
#include <memory>

namespace Ceres::AST {
enum class BinaryOp {
    Mult,
    Div,
    Modulo,
    Sum,
    Subtraction,
    BitshiftLeft,
    BitshiftRight,
    LessOrEqual,
    GreaterOrEqual,
    GreaterThan,
    LessThan,
    BitwiseAnd,
    BitwiseOr,
    BitwiseXor,
    Equals,
    NotEquals,
    LogicalAnd,
    LogicalOr
};

class BinaryOperationExpression : public Expression {
public:
    std::unique_ptr<Expression> left, right;
    BinaryOp op;

    SourceSpan opSpan;

    BinaryOperationExpression(SourceSpan const& sourceSpan, std::unique_ptr<Expression>&& left,
        std::unique_ptr<Expression>&& right, BinaryOp op, SourceSpan opSpan);

    void accept(AbstractASTVisitor& visitor) override;

    std::vector<Node*> getChildren() const override;
};

} // namespace Ceres::AST

#endif // COMPILER_BINARYOPERATIONEXPRESSION_H
