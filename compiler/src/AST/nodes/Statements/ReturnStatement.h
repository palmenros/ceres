#ifndef COMPILER_RETURNSTATEMENT_H
#define COMPILER_RETURNSTATEMENT_H

#include "../../../Binding/SymbolDeclaration.h"
#include "../Expressions/Expression.h"
#include "Statement.h"
#include <memory>
#include <optional>

namespace Ceres::AST {

class ReturnStatement : public Statement {
public:
    // May be null if no expression has been provided
    std::unique_ptr<Expression> expr;
    std::optional<Binding::SymbolDeclaration> decl;

    ReturnStatement(SourceSpan const& sourceSpan, std::unique_ptr<Expression>&& expr);

    void accept(AbstractASTVisitor& visitor) override;

    std::vector<Node*> getChildren() const override;
};

} // namespace Ceres::AST

#endif // COMPILER_RETURNSTATEMENT_H
