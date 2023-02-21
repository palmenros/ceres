#include "BinaryOperation.h"
#include <llvm/Support/Casting.h>

namespace Ceres::Typing {

BinaryOperation::BinaryOperation(BinaryOperation::Kind kind)
    : kind(kind)
{
}

BinaryOperation::BinaryOperation()
    : kind(Invalid)
{
}

Type* BinaryOperation::resTy(Type* lhs, Type* rhs)
{
    // Should already be of the same type
    // if (lhs != rhs) {
    //     return ErrorType::get();
    // }

    auto* typeInt = llvm::dyn_cast<PrimitiveIntegerType>(lhs);
    if (typeInt != nullptr) {
        switch (this->kind) {
        case BinaryOperation::Mult:
        case BinaryOperation::Div:
        case BinaryOperation::Sum:
        case BinaryOperation::Subtraction:
        case BinaryOperation::Modulo:
        case BinaryOperation::BitwiseAnd:
        case BinaryOperation::BitwiseOr:
        case BinaryOperation::BitwiseXor:
        case BinaryOperation::BitshiftLeft:
        case BinaryOperation::BitshiftRight: {
            return typeInt;
        }
        case BinaryOperation::LessOrEqual:
        case BinaryOperation::GreaterOrEqual:
        case BinaryOperation::GreaterThan:
        case BinaryOperation::LessThan:
        case BinaryOperation::Equals:
        case BinaryOperation::NotEquals: {
            return BoolType::get();
        }
        default:
            // TODO: print pretty error
            break;
        }
    }

    auto* typeB = llvm::dyn_cast<BoolType>(lhs);
    if (typeB != nullptr) {
        switch (this->kind) {
        case BinaryOperation::Equals:
        case BinaryOperation::NotEquals:
        case BinaryOperation::LogicalAnd:
        case BinaryOperation::LogicalOr: {
            return BoolType::get();
        }
        default:
            // TODO: print pretty error
            break;
        }
    }

    auto* typeF = llvm::dyn_cast<NotYetInferredType>(lhs);
    if (typeF != nullptr) {
        switch (this->kind) {
        case BinaryOperation::Mult:
        case BinaryOperation::Div:
        case BinaryOperation::Sum:
        case BinaryOperation::Subtraction: {
            return typeF;
        }
        // TODO: are floats actually comparable?
        case BinaryOperation::LessOrEqual:
        case BinaryOperation::GreaterOrEqual:
        case BinaryOperation::GreaterThan:
        case BinaryOperation::LessThan:
        case BinaryOperation::Equals:
        case BinaryOperation::NotEquals: {
            return BoolType::get();
        }
        default:
            // TODO: print pretty error
            break;
        }
    }

    Log::panic("Could not resolve binary expr type");
}

}