#include "BlockStatement.h"
#include "../../AbstractASTVisitor.h"

namespace Ceres::AST {

BlockStatement::BlockStatement(SourceSpan const& sourceSpan, std::vector<std::unique_ptr<Statement>>&& statements)
    : Statement(sourceSpan)
    , statements(std::move(statements))
{
}

void BlockStatement::accept(AbstractASTVisitor& visitor) { visitor.visitBlockStatement(*this); }

std::vector<Node*> BlockStatement::getChildren() const
{
    std::vector<Node*> v;
    v.reserve(statements.size());
    for (auto& a : statements) {
        v.push_back(a.get());
    }
    return v;
}
} // namespace Ceres::AST