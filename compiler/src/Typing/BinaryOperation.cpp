#include "BinaryOperation.h"
#include "Type.h"
#include <llvm/Support/Casting.h>
#include <spdlog/fmt/bundled/core.h>

namespace Ceres::Typing {

BinaryOperation::BinaryOperation(BinaryOperation::Kind kind)
    : kind(kind)
{
}

BinaryOperation::BinaryOperation()
    : kind(Invalid)
{
}

std::string BinaryOperation::toString() const
{
    switch (kind) {
    case Mult:
        return "*";
    case Div:
        return "/";
    case Modulo:
        return "%";
    case Sum:
        return "+";
    case Subtraction:
        return "-";
    case BitshiftLeft:
        return "<<";
    case BitshiftRight:
        return ">>";
    case LessOrEqual:
        return "<=";
    case GreaterOrEqual:
        return ">=";
    case GreaterThan:
        return ">";
    case LessThan:
        return "<";
    case BitwiseAnd:
        return "&";
    case BitwiseOr:
        return "|";
    case BitwiseXor:
        return "^";
    case Equals:
        return "==";
    case NotEquals:
        return "!=";
    case LogicalAnd:
        return "&&";
    case LogicalOr:
        return "||";
    case Invalid:
        return "Invalid";
    }
    NOT_IMPLEMENTED();
}

Type* BinaryOperation::resTy(Type* lhs) const
{
    auto* typeUnknown = llvm::dyn_cast<NotYetInferredType>(lhs);
    if (typeUnknown != nullptr) {
        switch (typeUnknown->kind) {
        case NotYetInferredKind::IntegerLiteral:
            // kind doesnt matter
            lhs = PrimitiveIntegerType::get(PrimitiveIntegerKind::I32);
            break;
        case NotYetInferredKind::FloatLiteral:
            // kind doesnt matter
            lhs = PrimitiveFloatType::get(PrimitiveFloatKind::F32);
            break;
        case NotYetInferredKind::VariableDeclaration:
        case NotYetInferredKind::Expression:
            Log::panic("Unreachable");
            break;
        }
    }

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
            return ErrorType::get();
        }
    }

    auto* typeBool = llvm::dyn_cast<BoolType>(lhs);
    if (typeBool != nullptr) {
        switch (this->kind) {
        case BinaryOperation::Equals:
        case BinaryOperation::NotEquals:
        case BinaryOperation::LogicalAnd:
        case BinaryOperation::LogicalOr: {
            return BoolType::get();
        }
        default:
            return ErrorType::get();
        }
    }

    auto* typeFloat = llvm::dyn_cast<PrimitiveFloatType>(lhs);
    if (typeFloat != nullptr) {
        switch (this->kind) {
        case BinaryOperation::Mult:
        case BinaryOperation::Div:
        case BinaryOperation::Sum:
        case BinaryOperation::Subtraction: {
            return typeFloat;
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
            return ErrorType::get();
        }
    }

    return ErrorType::get();
}

}