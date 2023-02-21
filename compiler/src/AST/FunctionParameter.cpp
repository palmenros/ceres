#include "FunctionParameter.h"

#include <utility>

namespace Ceres::AST {
FunctionParameter::FunctionParameter(Type* type, std::string name, Typing::Constness constness,
    SourceSpan typeSourceSpan, SourceSpan parameterNameSourceSpan)
    : type(type)
    , id(std::move(name))
    , constness(constness)
    , typeSourceSpan(typeSourceSpan)
    , parameterNameSourceSpan(parameterNameSourceSpan)
{
}
} // namespace Ceres::AST