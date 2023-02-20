#ifndef COMPILER_TYPEVISITOR_H
#define COMPILER_TYPEVISITOR_H

#include "Type.h"

namespace Ceres::Typing {

class TypeVisitor {
public:
    void visit(Type* type);

    // Note: This visitor uses pointers instead of references due to the
    //  unique nature of types. A type is uniquely represented by its
    //  pointer value, due to caching.

    virtual void visitUnitVoidType(UnitVoidType* type) = 0;
    virtual void visitUnresolvedType(UnresolvedType* type) = 0;
    virtual void visitNotYetInferredType(NotYetInferredType* type) = 0;
    virtual void visitPrimitiveScalarType(PrimitiveScalarType* type) = 0;
    virtual void visitFunctionType(FunctionType* type) = 0;
    virtual void visitErrorType(ErrorType* type) = 0;
};

} // namespace Ceres::Typing

#endif // COMPILER_TYPEVISITOR_H
