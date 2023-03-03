#ifndef COMPILER_BOOLLITERALEXPRESSION_H
#define COMPILER_BOOLLITERALEXPRESSION_H

#include "Expression.h"
#include <string>

namespace Ceres::AST {

enum class BoolLiteralValue { True, False };

class BoolLiteralExpression : public Expression {
public:
    BoolLiteralValue value;

    BoolLiteralExpression(SourceSpan const& sourceSpan, BoolLiteralValue value);

    void accept(AbstractASTVisitor& visitor) override;

    std::vector<Node*> getChildren() const override;

    static std::string toStringBoolLiteralValue(BoolLiteralValue value);
    bool getLiteralBool() const;
};

} // namespace Ceres::AST

#endif // COMPILER_BOOLLITERALEXPRESSION_H
