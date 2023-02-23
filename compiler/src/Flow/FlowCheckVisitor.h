#ifndef COMPILER_FlowCheckVisitor_H
#define COMPILER_FlowCheckVisitor_H

#include "../AST/ASTVisitor.h"
#include "../Binding/Scope.h"

namespace Ceres::Typing {

class FlowCheckVisitor : public AST::ASTVisitor {
public:
    void visitFunctionDefinition(AST::FunctionDefinition& def) override;
};
} // namespace Ceres::Typing

#endif // COMPILER_FlowCheckVisitor_H
