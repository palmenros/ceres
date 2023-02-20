#include "FunctionParameter.h"

#include <utility>

namespace Ceres::AST {
FunctionParameter::FunctionParameter(Type* type, std::string name, VariableConstness constness,
    SourceSpan typeSourceSpan, SourceSpan parameterNameSourceSpan)
    : type(type)
    , name(std::move(name))
    , constness(constness)
    , typeSourceSpan(typeSourceSpan)
    , parameterNameSourceSpan(parameterNameSourceSpan)
{
}
} // namespace Ceres::AST