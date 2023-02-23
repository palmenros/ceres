#include "FlowCheckVisitor.h"
#include "../Diagnostics/Diagnostics.h"
#include <llvm/Support/Casting.h>

namespace Ceres::Typing {

FlowData FlowCheckVisitor::doVisitFunctionDefinition(AST::FunctionDefinition& def)
{
    auto blockData = visit(*def.block);
    auto functionReturnsVoid = llvm::isa<VoidType>(def.returnType);

    if (!functionReturnsVoid && !blockData.allCodePathsHaveReturnStatements) {
        // TODO: Maybe save the inner SourceSpan (end of block) of the first block that doesn't return to
        // give better error messages
        Diagnostics::report(def.sourceSpan, Diag::no_return_on_non_void);
    }

    return blockData;
}
FlowData FlowCheckVisitor::doVisitBlockStatement(AST::BlockStatement& blockStm)
{
    for (auto const& stm : blockStm.statements) {
        auto data = visit(*stm);
        if (data.allCodePathsHaveReturnStatements) {
            // The only statement that matters is the first one
            return FlowData { true };
        }
    }
    return FlowData { false };
}

FlowData FlowCheckVisitor::doVisitExpressionStatement(AST::ExpressionStatement& stm)
{
    // Expressions don't return
    return FlowData { false };
}

FlowData FlowCheckVisitor::doVisitForStatement(AST::ForStatement& stm)
{
    // Loop may never be executed, so return that not all code path have return statements
    return FlowData { false };
}

FlowData FlowCheckVisitor::doVisitIfStatement(AST::IfStatement& stm)
{
    auto thenData = visit(*stm.thenBlock);

    if (stm.maybeElseStatement != nullptr) {
        auto elseData = visit(*stm.maybeElseStatement);
        return FlowData { thenData.allCodePathsHaveReturnStatements && elseData.allCodePathsHaveReturnStatements };
    } else {
        return FlowData { thenData.allCodePathsHaveReturnStatements };
    }
}

FlowData FlowCheckVisitor::doVisitReturnStatement(AST::ReturnStatement& stm) { return FlowData { true }; }

FlowData FlowCheckVisitor::doVisitWhileStatement(AST::WhileStatement& stm)
{
    // Loop may never be executed, so return that not all code path have return statements
    return FlowData { false };
}

} // namespace Ceres::Typing
