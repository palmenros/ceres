#ifndef COMPILER_PREFIXEXPRESSION_H
#define COMPILER_PREFIXEXPRESSION_H

#include "Expression.h"
#include <memory>

namespace Ceres::AST {

enum class PrefixOp { PrefixIncrement, PrefixDecrement, UnaryPlus, UnaryMinus, UnaryLogicalNot, UnaryBitwiseNot };

class PrefixExpression : public Expression {
public:
    PrefixOp op;
    std::unique_ptr<Expression> expr;

    SourceSpan opSourceSpan;

    PrefixExpression(
        SourceSpan const& sourceSpan, PrefixOp op, std::unique_ptr<Expression>&& expr, SourceSpan const& opSourceSpan);

    void accept(AbstractASTVisitor& visitor) override;

    std::vector<Node*> getChildren() const override;
};

} // namespace Ceres::AST

#endif // COMPILER_PREFIXEXPRESSION_H
