#ifndef COMPILER_BINDINGVISITOR_H
#define COMPILER_BINDINGVISITOR_H

#include "../AST/ASTVisitor.h"
#include "Scope.h"

namespace Ceres::Binding {

class BindingVisitor : public AST::ASTVisitor {
public:
    std::vector<AST::IdentifierExpression*> unresolvedScope;
    Scope* currentScope = nullptr;

    // Since we don't resolve anything in this pass we just have to add the
    // defined names to the tables
    void visitCompilationUnit(AST::CompilationUnit& unit) override;
    void visitBlockStatement(AST::BlockStatement& stm) override;
    void visitFunctionDefinition(AST::FunctionDefinition& def) override;
    void visitVariableDeclaration(AST::VariableDeclaration& decl) override;
    void visitIdentifierExpression(AST::IdentifierExpression& expr) override;
    void visitAssignmentExpression(AST::AssignmentExpression& expr) override;

    // TODO: think about this
    // void visitForStatement(ForStatement &stm) override;
};
} // namespace Ceres::Binding

#endif // COMPILER_BINDINGVISITOR_H
