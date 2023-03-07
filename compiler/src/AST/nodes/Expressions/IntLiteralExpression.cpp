#include "IntLiteralExpression.h"
#include "../../AbstractASTVisitor.h"
#include <cstddef>
#include <cstdlib>
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
    auto base = getRadix();

    // TODO: should be correct or caught in parser
    char* end = nullptr;

    if (intType->isSigned()) {
        auto res = std::strtoll(str.c_str(), &end, base);

        if (errno == ERANGE) {
            errno = 0;
            return false;
        }

        // check bits fit
        auto width = intType->getNumBits();
        long long mask = ~0 << width;
        long long last_bit = 1 << (width - 1);

        // assume 2 complement
        if ((res & last_bit) == 0 && (res & mask) != 0) {
            return false;
        }

        if ((res & last_bit) == 1 && (res | ~mask) != ~0) {
            return false;
        }
    } else {
        auto res = std::strtoull(str.c_str(), &end, base);

        if (errno == ERANGE) {
            errno = 0;
            return false;
        }

        // check bits fit
        auto width = intType->getNumBits();

        unsigned long long mask = ~0 << width;

        if ((res & mask) != 0) {
            return false;
        }
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