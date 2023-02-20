#ifndef COMPILER_IDENTIFIEREXPRESSION_H
#define COMPILER_IDENTIFIEREXPRESSION_H

#include "Expression.h"
#include <string>

namespace Ceres::AST {

class IdentifierExpression : public Expression {
public:
    std::string identifier;

    IdentifierExpression(SourceSpan const& sourceSpan, std::string identifier);

    void accept(AbstractASTVisitor& visitor) override;

    std::vector<Node*> getChildren() const override;
};

} // namespace Ceres::AST

#endif // COMPILER_IDENTIFIEREXPRESSION_H
