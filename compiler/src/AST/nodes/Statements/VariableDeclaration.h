#ifndef COMPILER_VARIABLEDECLARATION_H
#define COMPILER_VARIABLEDECLARATION_H

#include "../../../Typing/Constness.h"
#include "../../../Typing/Type.h"
#include "../../../Typing/Visibility.h"
#include "../Expressions/Expression.h"
#include "Statement.h"
#include <memory>
#include <string>

namespace Ceres::AST {

enum class VariableScope {
    Local, // Defined inside a function
    Global // Defined as a global variable, outside any function
};

class VariableDeclaration : public Statement {
public:
    Typing::VariableVisibility visibility;
    Typing::Constness constness;
    VariableScope scope;

    Type* type;
    std::string identifier;

    SourceSpan typeSourceSpan;
    SourceSpan identifierSourceSpan;

    // Can be nullptr if the variable doesn't have initializer expression
    std::unique_ptr<Expression> initializerExpression;

public:
    VariableDeclaration(Ceres::SourceSpan const& sourceSpan, std::unique_ptr<Expression>&& initializerExpression,
        Typing::VariableVisibility visibility, Typing::Constness constness, VariableScope scope, Type* type,
        std::string identifier, SourceSpan const& typeSourceSpan, SourceSpan const& identifierSourceSpan);

    void accept(AbstractASTVisitor& visitor) override;

    std::vector<Node*> getChildren() const override;
};
} // namespace Ceres::AST

#endif // COMPILER_VARIABLEDECLARATION_H
