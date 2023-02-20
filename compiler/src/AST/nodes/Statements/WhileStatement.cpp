#include "WhileStatement.h"
#include "../../AbstractASTVisitor.h"
#include "BlockStatement.h"

namespace Ceres::AST {
WhileStatement::WhileStatement(
    SourceSpan const& sourceSpan, std::unique_ptr<Expression>&& condition, std::unique_ptr<BlockStatement>&& body)
    : Statement(sourceSpan)
    , condition(std::move(condition))
    , body(std::move(body))
{
}

void WhileStatement::accept(AbstractASTVisitor& visitor) { visitor.visitWhileStatement(*this); }

std::vector<Node*> WhileStatement::getChildren() const { return { condition.get(), body.get() }; }
} // namespace Ceres::AST