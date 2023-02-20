#ifndef COMPILER_IFSTATEMENT_H
#define COMPILER_IFSTATEMENT_H

#include "../Expressions/Expression.h"
#include "BlockStatement.h"
#include "Statement.h"
#include <memory>

namespace Ceres::AST {

class IfStatement : public Statement {
public:
    std::unique_ptr<Expression> condition;

    std::unique_ptr<BlockStatement> thenBlock;

    // Currently: the else statement can only be a BlockStatement, another
    // IfStatement or a nullptr Note: maybeElseStatement can be a nullptr
    std::unique_ptr<Statement> maybeElseStatement;

    IfStatement(SourceSpan const& sourceSpan, std::unique_ptr<Expression>&& condition,
        std::unique_ptr<BlockStatement>&& thenBlock, std::unique_ptr<Statement>&& elseStatement);

    void accept(AbstractASTVisitor& visitor) override;

    std::vector<Node*> getChildren() const override;
};

} // namespace Ceres::AST

#endif // COMPILER_IFSTATEMENT_H
