/*
 * Copyright (C) 2023 Pedro Palacios Almendros
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef COMPILER_ASSIGNMENTEXPRESSION_H
#define COMPILER_ASSIGNMENTEXPRESSION_H

#include "BinaryOperationExpression.h"
#include "Expression.h"
#include <optional>
#include <string>

namespace Ceres::AST {

class AssignmentExpression : public Expression {
public:
    // We embed the binary operation inside the Assignment Expresson because the
    // LHS (identifier) is the same, which can help us optimize the code better.
    // However, we may not have a binary op in the assignment, so it is an
    // optional type
    std::optional<BinaryOp> binaryOp;

    // Identifier of the variable that is being assigned
    std::string identifierLHS;

    // Expression that is being assigned (possibly after performing a binary
    // operation)
    // TODO: nullable?
    std::unique_ptr<Expression> expressionRHS;

    SourceSpan opSourceSpan;

    AssignmentExpression(SourceSpan const& sourceSpan, std::optional<BinaryOp> const& binaryOp,
        std::string identifierLhs, std::unique_ptr<Expression>&& expressionRhs, SourceSpan opSourceSpan);

    void accept(AbstractASTVisitor& visitor) override;

    std::vector<Node*> getChildren() const override;
};

} // namespace Ceres::AST

#endif // COMPILER_ASSIGNMENTEXPRESSION_H
