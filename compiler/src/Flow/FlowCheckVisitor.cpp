#include "FlowCheckVisitor.h"
#include "../Binding/Scope.h"
#include "../Diagnostics/Diagnostics.h"
#include "../Typing/BinaryOperation.h"
#include "../Typing/Type.h"
#include <cstddef>
#include <llvm/Support/Casting.h>
#include <optional>
#include <spdlog/fmt/bundled/core.h>

namespace Ceres::Typing {

void FlowCheckVisitor::visitFunctionDefinition(AST::FunctionDefinition& def)
{
    // Returning something inside a void function is catched in type check
    auto has_return = false;
    auto is_void = llvm::isa<VoidType>(def.returnType);

    if (!is_void && llvm::isa<VoidType>(def.block->type)) {
        Diagnostics::report(def.sourceSpan, Diag::no_return_on_non_void);
    }
}

} // namespace Ceres::Typing
