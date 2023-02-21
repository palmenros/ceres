#ifndef COMPILER_BINARYOPERATION_H
#define COMPILER_BINARYOPERATION_H

#include "Type.h"
#include <memory>
#include <string>

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
    std::string toString() const;

    Type* resTy(Type* lhs) const;
};

}

#endif // COMPILER_BINARYOPERATION_H
