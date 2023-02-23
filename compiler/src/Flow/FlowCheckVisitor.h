#ifndef COMPILER_FlowCheckVisitor_H
#define COMPILER_FlowCheckVisitor_H

#include "../AST/ASTVisitor.h"
#include "../AST/ReturningASTVisitor.hpp"
#include "../Binding/Scope.h"

namespace Ceres::Typing {

struct FlowData {
    /* Default value */
    bool allCodePathsHaveReturnStatements = false;
};

class FlowCheckVisitor : public AST::ReturningASTVisitor<FlowData> {
public:
    FlowData doVisitFunctionDefinition(AST::FunctionDefinition& def) override;

    FlowData doVisitBlockStatement(AST::BlockStatement& stm) override;
    FlowData doVisitExpressionStatement(AST::ExpressionStatement& stm) override;
    FlowData doVisitForStatement(AST::ForStatement& stm) override;
    FlowData doVisitIfStatement(AST::IfStatement& stm) override;
    FlowData doVisitReturnStatement(AST::ReturnStatement& stm) override;
    FlowData doVisitWhileStatement(AST::WhileStatement& stm) override;
};
} // namespace Ceres::Typing

#endif // COMPILER_FlowCheckVisitor_H
