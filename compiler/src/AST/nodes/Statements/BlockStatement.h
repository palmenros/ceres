#ifndef COMPILER_BLOCKSTATEMENT_H
#define COMPILER_BLOCKSTATEMENT_H

#include "../../../Binding/Scope.h"
#include "Statement.h"
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

namespace Ceres::AST {
class BlockStatement : public Statement {

public:
    bool always_returns = false;

    std::vector<std::unique_ptr<Statement>> statements;

    BlockStatement(SourceSpan const& sourceSpan, std::vector<std::unique_ptr<Statement>>&& statements);

    void accept(AbstractASTVisitor& visitor) override;

    std::vector<Node*> getChildren() const override;
};
} // namespace Ceres::AST

#endif // COMPILER_BLOCKSTATEMENT_H
