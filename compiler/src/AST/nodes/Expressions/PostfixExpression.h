#ifndef COMPILER_POSTFIXEXPRESSION_H
#define COMPILER_POSTFIXEXPRESSION_H

#include "Expression.h"
#include <memory>

namespace Ceres::AST {
enum class PostfixOp { PostfixIncrement, PostfixDecrement };

std::string postfixOpToString(PostfixOp op);

class PostfixExpression : public Expression {
public:
    PostfixOp op;
    std::unique_ptr<Expression> expr;

    SourceSpan opSourceSpan;

    PostfixExpression(
        SourceSpan const& sourceSpan, PostfixOp op, std::unique_ptr<Expression>&& expr, SourceSpan opSourceSpan);

    void accept(AbstractASTVisitor& visitor) override;

    std::vector<Node*> getChildren() const override;
};

} // namespace Ceres::AST

#endif // COMPILER_POSTFIXEXPRESSION_H
