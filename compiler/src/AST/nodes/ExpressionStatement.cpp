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

#include "ExpressionStatement.h"
#include "../AbstractASTVisitor.h"

namespace Ceres::AST {
        ExpressionStatement::ExpressionStatement(SourceSpan &&sourceSpan,
                                                 std::unique_ptr<Expression>&&expression) : Statement(sourceSpan),
                                                                                                  expression(std::move(expression)) {}

    void ExpressionStatement::accept(AbstractASTVisitor &visitor) {
        visitor.visitExpressionStatement(*this);
    }

    std::vector<Node *> ExpressionStatement::getChildren() const {
        return {expression.get()};
    }
} // AST