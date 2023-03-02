#include "IntLiteralExpression.h"
#include "../../AbstractASTVisitor.h"
#include <llvm/ADT/APInt.h>
#include <unordered_map>

namespace Ceres::AST {

IntLiteralExpression::IntLiteralExpression(
    SourceSpan const& sourceSpan, IntLiteralBase base, Type* type, std::string str)
    : Expression(sourceSpan, type)
    , base(base)
    , str(std::move(str))
{
}

void IntLiteralExpression::accept(AbstractASTVisitor& visitor) { visitor.visitIntLiteralExpression(*this); }

std::vector<Node*> IntLiteralExpression::getChildren() const { return {}; }

bool IntLiteralExpression::doesLiteralFitInsideType()
{
    // TODO: Call this function where appropriate in TypingVisitor

    // TODO: This function is not accurate enough. Only checks for some condition. We should improve it
    auto* intType = llvm::dyn_cast<PrimitiveIntegerType>(type);
    ASSERT(intType != nullptr);

    unsigned numBits = intType->getNumBits();
    uint8_t radix = getRadix();

    size_t stringLength = str.size();

    if (!(stringLength <= numBits || radix != 2)) {
        return false;
    }

    if (!((stringLength - 1) * 3 <= numBits || radix != 8)) {
        return false;
    }

    if (!((stringLength - 1) * 4 <= numBits || radix != 16)) {
        return false;
    }

    if (!(((stringLength - 1) * 64) / 22 <= numBits || radix != 10)) {
        return false;
    }

    return true;
}

llvm::APInt IntLiteralExpression::getLLVMAPInt()
{
    ASSERT(doesLiteralFitInsideType());

    uint8_t radix = getRadix();
    auto* intType = llvm::dyn_cast<PrimitiveIntegerType>(type);
    ASSERT(intType != nullptr);

    unsigned numBits = intType->getNumBits();

    return llvm::APInt(numBits, str, radix);
}

uint8_t IntLiteralExpression::getRadix() const
{
    uint8_t radix;
    switch (base) {
    case IntLiteralBase::Dec:
        radix = 10;
        break;
    case IntLiteralBase::Hex:
        radix = 16;
        break;
    case IntLiteralBase::Oct:
        radix = 8;
        break;
    case IntLiteralBase::Bin:
        radix = 2;
        break;
    default:
        NOT_IMPLEMENTED();
    }
    return radix;
}

} // namespace Ceres::AST