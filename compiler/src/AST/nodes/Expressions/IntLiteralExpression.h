#ifndef COMPILER_INTLITERALEXPRESSION_H
#define COMPILER_INTLITERALEXPRESSION_H

#include "../../../Typing/Type.h"
#include "Expression.h"
#include <memory>
#include <string>

namespace Ceres::AST {

enum class IntLiteralBase { Dec, Hex, Oct, Bin };

class IntLiteralExpression : public Expression {
public:
    IntLiteralBase base;

    // String containing the integer literal in base "base" without the
    // introducer characters for the base and the type. For example: 0xfc783u32
    // -> fc783
    std::string str;

    IntLiteralExpression(SourceSpan const& sourceSpan, IntLiteralBase base, Type* type, std::string str);

    llvm::APInt getLLVMAPInt();

    void accept(AbstractASTVisitor& visitor) override;

    std::vector<Node*> getChildren() const override;
    bool doesLiteralFitInsideType();
    uint8_t getRadix() const;
};

} // namespace Ceres::AST

#endif // COMPILER_INTLITERALEXPRESSION_H
