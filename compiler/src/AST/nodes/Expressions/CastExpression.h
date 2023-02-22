#ifndef COMPILER_CASTEXPRESSION_H
#define COMPILER_CASTEXPRESSION_H

#include "../../../Typing/Type.h"
#include "Expression.h"
#include <memory>

namespace Ceres::AST  {

class CastExpression : public Expression {
public:

    std::unique_ptr<Expression> expr;

    // Type to cast to
    Type* destinationType;

    CastExpression(const SourceSpan & sourceSpan, std::unique_ptr<Expression>&& expr, Type* castToType);

    void accept(AbstractASTVisitor& visitor) override;
    std::vector<Node*> getChildren() const override;
};

} // AST

#endif // COMPILER_CASTEXPRESSION_H
