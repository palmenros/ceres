#ifndef COMPILER_FlowCheckVisitor_H
#define COMPILER_FlowCheckVisitor_H

#include "../AST/ASTVisitor.h"
#include "../Binding/Scope.h"

namespace Ceres::Typing {

class FlowCheckVisitor : public AST::ASTVisitor {
public:
    void visitFunctionDefinition(AST::FunctionDefinition& def) override;
    // void visitIfStatement(AST::IfStatement& stm) override;
    // void visitBlockStatement(AST::BlockStatement& stm) override;
};
} // namespace Ceres::Typing

#endif // COMPILER_FlowCheckVisitor_H
