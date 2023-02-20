#ifndef COMPILER_EXPRESSIONSTATEMENT_H
#define COMPILER_EXPRESSIONSTATEMENT_H

#include "../Expressions/Expression.h"
#include "Statement.h"
#include <memory>

namespace Ceres::AST {

class ExpressionStatement : public Statement {
public:
    std::unique_ptr<Expression> expression;

    ExpressionStatement(SourceSpan&& sourceSpan, std::unique_ptr<Expression>&& expression);

    void accept(AbstractASTVisitor& visitor) override;

    std::vector<Node*> getChildren() const override;
};

} // namespace Ceres::AST

#endif // COMPILER_EXPRESSIONSTATEMENT_H
