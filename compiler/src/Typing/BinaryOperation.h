#ifndef COMPILER_BINARYOPERATION_H
#define COMPILER_BINARYOPERATION_H

#include "Type.h"

namespace Ceres::Typing {

class BinaryOperation {
public:
    enum Kind {
        Mult,
        Div,
        Modulo,
        Sum,
        Subtraction,
        BitshiftLeft,
        BitshiftRight,
        LessOrEqual,
        GreaterOrEqual,
        GreaterThan,
        LessThan,
        BitwiseAnd,
        BitwiseOr,
        BitwiseXor,
        Equals,
        NotEquals,
        LogicalAnd,
        LogicalOr,

        Invalid
    };

    Kind kind;
    BinaryOperation(Kind kind);
    BinaryOperation();

    Type* resTy(Type* lhs, Type* rhs);
};

}
#include "../AST/nodes/Expressions/Expression.h"
#include <memory>
#endif // COMPILER_BINARYOPERATION_H
