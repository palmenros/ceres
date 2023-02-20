
#include "Visibility.h"

namespace Ceres::Typing {

Typing::VariableVisibility::VariableVisibility(Typing::VariableVisibility::Kind kind)
    : kind(kind)
{
}

VariableVisibility::VariableVisibility()
    : kind(Invalid)
{
}

FunctionVisibility::FunctionVisibility(FunctionVisibility::Kind kind)
    : kind(kind)
{
}
FunctionVisibility::FunctionVisibility()
    : kind(Invalid)
{
}

}