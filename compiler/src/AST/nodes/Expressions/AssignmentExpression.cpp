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

#include "AssignmentExpression.h"
#include "../../../utils/log.hpp"
#include "../../AbstractASTVisitor.h"
#include <utility>

namespace Ceres::AST {
AssignmentExpression::AssignmentExpression(
    const SourceSpan &sourceSpan, const std::optional<BinaryOp> &binaryOp,
    std::string identifierLhs, std::unique_ptr<Expression> &&expressionRhs,
    SourceSpan opSourceSpan)
    : Expression(sourceSpan), binaryOp(binaryOp),
      identifierLHS(std::move(identifierLhs)),
      expressionRHS(std::move(expressionRhs)), opSourceSpan(opSourceSpan) {}

void AssignmentExpression::accept(AbstractASTVisitor &visitor) {
    visitor.visitAssignmentExpression(*this);
}

std::vector<Node *> AssignmentExpression::getChildren() const {
    // TODO: We will need to update this when our assignment expression takes an
    // expression instead of identifier on the LHS
    return {expressionRHS.get()};
}
} // namespace Ceres::AST