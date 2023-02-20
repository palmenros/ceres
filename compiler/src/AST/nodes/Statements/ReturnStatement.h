#ifndef COMPILER_RETURNSTATEMENT_H
#define COMPILER_RETURNSTATEMENT_H

#include "../Expressions/Expression.h"
#include "Statement.h"
#include <memory>

namespace Ceres::AST {

class ReturnStatement : public Statement {
public:
    std::unique_ptr<Expression> expr;

    ReturnStatement(SourceSpan const& sourceSpan, std::unique_ptr<Expression>&& expr);

    void accept(AbstractASTVisitor& visitor) override;

    std::vector<Node*> getChildren() const override;
};

} // namespace Ceres::AST

#endif // COMPILER_RETURNSTATEMENT_H
