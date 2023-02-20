#ifndef COMPILER_FLOATLITERALEXPRESSION_H
#define COMPILER_FLOATLITERALEXPRESSION_H

#include "../../../Typing/Type.h"
#include "Expression.h"
#include <memory>
#include <string>

namespace Ceres::AST {

enum class FloatLiteralBase { Dec, Hex };

class FloatLiteralExpression : public Expression {
public:
    FloatLiteralBase base;

    // String containing the integer literal in base "base" without the
    // introducer characters for the base and the type. For example: 0xfc783u32
    // -> fc783
    std::string str;

    FloatLiteralExpression(SourceSpan const& sourceSpan, FloatLiteralBase base, Type* type, std::string str);

    void accept(AbstractASTVisitor& visitor) override;

    std::vector<Node*> getChildren() const override;
};

} // namespace Ceres::AST

#endif // COMPILER_FLOATLITERALEXPRESSION_H
