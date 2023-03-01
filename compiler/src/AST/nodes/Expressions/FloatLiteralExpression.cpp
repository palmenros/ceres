#include "FloatLiteralExpression.h"

#include "../../AbstractASTVisitor.h"
#include <llvm/ADT/APFloat.h>
#include <utility>

namespace Ceres::AST {

FloatLiteralExpression::FloatLiteralExpression(
    SourceSpan const& sourceSpan, FloatLiteralBase base, Type* type, std::string str)
    : Expression(sourceSpan, type)
    , base(base)
    , str(std::move(str))
{
}

void FloatLiteralExpression::accept(AbstractASTVisitor& visitor) { visitor.visitFloatLiteralExpression(*this); }

std::vector<Node*> FloatLiteralExpression::getChildren() const { return {}; }

llvm::APFloat FloatLiteralExpression::getLLVMAPFloat()
{
    if (base != FloatLiteralBase::Dec) {
        NOT_IMPLEMENTED();
    }

    PrimitiveFloatType* floatType = llvm::dyn_cast<PrimitiveFloatType>(type);
    ASSERT(floatType != nullptr);

    switch (floatType->kind) {
    case PrimitiveFloatKind::F32:
        return llvm::APFloat(llvm::APFloat::IEEEsingle(), str);
    case PrimitiveFloatKind::F64:
        return llvm::APFloat(llvm::APFloat::IEEEdouble(), str);
    default:
        NOT_IMPLEMENTED();
    }
}
} // namespace Ceres::AST