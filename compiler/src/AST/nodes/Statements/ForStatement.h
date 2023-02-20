#ifndef COMPILER_FORSTATEMENT_H
#define COMPILER_FORSTATEMENT_H

#include "../Expressions/Expression.h"
#include "BlockStatement.h"
#include "Statement.h"
#include "VariableDeclaration.h"
#include <memory>

namespace Ceres::AST {

class ForStatement : public Statement {
public:
    // Note: Any expression in the for except the block can be a nullptr (for
    // (;;;) {}). moreover, For may have as its first argument either a
    // declaration or an expression: It may have a declaration (for(let i =
    // 0;;){}) or an expression (let i : i32; for(i = 0;;){}). At most one of
    // them will be a non nullptr, but both may be nullptr.
    std::unique_ptr<VariableDeclaration> maybeInitDeclaration;
    std::unique_ptr<Expression> maybeInitExpression;

    std::unique_ptr<Expression> maybeConditionExpr;
    std::unique_ptr<Expression> maybeUpdateExpr;

    std::unique_ptr<Ceres::AST::BlockStatement> body;

    ForStatement(SourceSpan const& sourceSpan, std::unique_ptr<VariableDeclaration>&& maybeInitDeclaration,
        std::unique_ptr<Expression>&& maybeInitExpression, std::unique_ptr<Expression>&& conditionExpr,
        std::unique_ptr<Expression>&& updateExpr, std::unique_ptr<Ceres::AST::BlockStatement>&& body);

    void accept(AbstractASTVisitor& visitor) override;

    std::vector<Node*> getChildren() const override;
};

} // namespace Ceres::AST

#endif // COMPILER_FORSTATEMENT_H
