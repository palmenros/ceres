#include "AssignmentExpression.h"
#include "../../../utils/log.hpp"
#include "../../AbstractASTVisitor.h"
#include <utility>

namespace Ceres::AST {
AssignmentExpression::AssignmentExpression(SourceSpan const& sourceSpan, std::optional<BinaryOp> const& binaryOp,
    std::string identifierLhs, std::unique_ptr<Expression>&& expressionRhs, SourceSpan opSourceSpan)
    : Expression(sourceSpan)
    , binaryOp(binaryOp)
    , identifierLHS(std::move(identifierLhs))
    , expressionRHS(std::move(expressionRhs))
    , opSourceSpan(opSourceSpan)
{
}

void AssignmentExpression::accept(AbstractASTVisitor& visitor) { visitor.visitAssignmentExpression(*this); }

std::vector<Node*> AssignmentExpression::getChildren() const
{
    // TODO: We will need to update this when our assignment expression takes an
    // expression instead of identifier on the LHS
    return { expressionRHS.get() };
}
} // namespace Ceres::AST