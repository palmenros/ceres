#include "IfStatement.h"
#include "../../AbstractASTVisitor.h"

namespace Ceres::AST {
IfStatement::IfStatement(SourceSpan const& sourceSpan, std::unique_ptr<Expression>&& condition,
    std::unique_ptr<BlockStatement>&& thenBlock, std::unique_ptr<Statement>&& elseStatement)
    : Statement(sourceSpan)
    , condition(std::move(condition))
    , thenBlock(std::move(thenBlock))
    , maybeElseStatement(std::move(elseStatement))
{
}

void IfStatement::accept(AbstractASTVisitor& visitor) { visitor.visitIfStatement(*this); }

std::vector<Node*> IfStatement::getChildren() const
{
    std::vector<Node*> v;
    v.push_back(condition.get());
    v.push_back(thenBlock.get());
    if (maybeElseStatement != nullptr) {
        v.push_back(maybeElseStatement.get());
    }
    return v;
}

} // namespace Ceres::AST