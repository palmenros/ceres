#include "AssignmentExpression.h"
#include "../../../Typing/BinaryOperation.h"
#include "../../../utils/log.hpp"
#include "../../AbstractASTVisitor.h"
#include <utility>

namespace Ceres::AST {
AssignmentExpression::AssignmentExpression(SourceSpan const& sourceSpan,
    std::optional<Typing::BinaryOperation> const& binaryOp, std::unique_ptr<Expression>&& expressionLhs,
    std::unique_ptr<Expression>&& expressionRhs, SourceSpan opSourceSpan)
    : Expression(sourceSpan)
    , binaryOp(binaryOp)
    , expressionLHS(std::move(expressionLhs))
    , expressionRHS(std::move(expressionRhs))
    , opSourceSpan(opSourceSpan)
{
}

void AssignmentExpression::accept(AbstractASTVisitor& visitor) { visitor.visitAssignmentExpression(*this); }

std::vector<Node*> AssignmentExpression::getChildren() const { return { expressionLHS.get(), expressionRHS.get() }; }
} // namespace Ceres::AST