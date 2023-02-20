#ifndef COMPILER_WHILESTATEMENT_H
#define COMPILER_WHILESTATEMENT_H

#include "../Expressions/Expression.h"
#include "BlockStatement.h"
#include "Statement.h"
#include <memory>

namespace Ceres::AST {

class WhileStatement : public Statement {
public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Ceres::AST::BlockStatement> body;

    WhileStatement(SourceSpan const& sourceSpan, std::unique_ptr<Expression>&& condition,
        std::unique_ptr<Ceres::AST::BlockStatement>&& body);

    void accept(AbstractASTVisitor& visitor) override;

    std::vector<Node*> getChildren() const override;
};

} // namespace Ceres::AST

#endif // COMPILER_WHILESTATEMENT_H
