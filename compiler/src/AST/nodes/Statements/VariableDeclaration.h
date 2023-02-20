#ifndef COMPILER_VARIABLEDECLARATION_H
#define COMPILER_VARIABLEDECLARATION_H

#include "../../../Typing/Type.h"
#include "../Expressions/Expression.h"
#include "Statement.h"
#include <memory>
#include <string>

namespace Ceres::AST {
enum class VariableVisibility { Public, Private };

enum class VariableScope {
    Local, // Defined inside a function
    Global // Defined as a global variable, outside any function
};

enum class VariableConstness { Const, NonConst };

class VariableDeclaration : public Statement {
public:
    VariableVisibility visibility;
    VariableConstness constness;
    VariableScope scope;

    Type* type;
    std::string identifier;

    SourceSpan typeSourceSpan;
    SourceSpan identifierSourceSpan;

    // Can be nullptr if the variable doesn't have initializer expression
    std::unique_ptr<Expression> initializerExpression;

public:
    VariableDeclaration(Ceres::SourceSpan const& sourceSpan, std::unique_ptr<Expression>&& initializerExpression,
        VariableVisibility visibility, VariableConstness constness, VariableScope scope, Type* type,
        std::string identifier, SourceSpan const& typeSourceSpan, SourceSpan const& identifierSourceSpan);

    void accept(AbstractASTVisitor& visitor) override;

    std::vector<Node*> getChildren() const override;
};
} // namespace Ceres::AST

#endif // COMPILER_VARIABLEDECLARATION_H
