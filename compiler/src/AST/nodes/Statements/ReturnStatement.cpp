#include "ReturnStatement.h"
#include "../../AbstractASTVisitor.h"

namespace Ceres::AST {
ReturnStatement::ReturnStatement(SourceSpan const& sourceSpan, std::unique_ptr<Expression>&& expr)
    : Statement(sourceSpan)
    , expr(std::move(expr))
{
}

void ReturnStatement::accept(AbstractASTVisitor& visitor) { visitor.visitReturnStatement(*this); }

std::vector<Node*> ReturnStatement::getChildren() const { return { expr.get() }; }
} // namespace Ceres::AST