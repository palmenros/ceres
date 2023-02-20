#ifndef COMPILER_ASSIGNMENTEXPRESSION_H
#define COMPILER_ASSIGNMENTEXPRESSION_H

#include "../../../Typing/BinaryOperation.h"
#include "BinaryOperationExpression.h"
#include "Expression.h"
#include <optional>
#include <string>

namespace Ceres::AST {

class AssignmentExpression : public Expression {
public:
    // We embed the binary operation inside the Assignment Expresson because the
    // LHS (identifier) is the same, which can help us optimize the code better.
    // However, we may not have a binary op in the assignment, so it is an
    // optional type
    std::optional<Typing::BinaryOperation> binaryOp;

    // Identifier of the variable that is being assigned.  Not nullable.
    std::unique_ptr<Expression> expressionLHS;

    // Expression that is being assigned (possibly after performing a binary
    // operation). Not nullable.
    std::unique_ptr<Expression> expressionRHS;

    SourceSpan opSourceSpan;

    AssignmentExpression(SourceSpan const& sourceSpan, std::optional<Typing::BinaryOperation> const& binaryOp,
        std::unique_ptr<Expression>&& expressionLhs, std::unique_ptr<Expression>&& expressionRhs,
        SourceSpan opSourceSpan);

    void accept(AbstractASTVisitor& visitor) override;

    std::vector<Node*> getChildren() const override;
};

} // namespace Ceres::AST

#endif // COMPILER_ASSIGNMENTEXPRESSION_H
