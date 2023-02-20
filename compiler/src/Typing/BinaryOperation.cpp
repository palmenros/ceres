#include "BinaryOperation.h"

namespace Ceres::Typing {

BinaryOperation::BinaryOperation(BinaryOperation::Kind kind)
    : kind(kind)
{
}

BinaryOperation::BinaryOperation()
    : kind(Invalid)
{
}
}