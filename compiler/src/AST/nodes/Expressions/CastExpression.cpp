#include "CastExpression.h"
#include "../../AbstractASTVisitor.h"

namespace Ceres::AST {

CastExpression::CastExpression(SourceSpan const& sourceSpan, std::unique_ptr<Expression>&& expr, Type* castToType)
    : Expression(sourceSpan)
    , expr(std::move(expr))
    , destinationType(castToType)
{
}
void CastExpression::accept(AbstractASTVisitor& visitor) { visitor.visitCastExpression(*this); }

std::vector<Node*> CastExpression::getChildren() const { return { expr.get() }; }

} // AST