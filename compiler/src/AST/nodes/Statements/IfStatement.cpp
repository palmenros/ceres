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

#include "IfStatement.h"
#include "../../AbstractASTVisitor.h"

namespace Ceres::AST {
IfStatement::IfStatement(const SourceSpan &sourceSpan,
                         std::unique_ptr<Expression> &&condition,
                         std::unique_ptr<BlockStatement> &&thenBlock,
                         std::unique_ptr<Statement> &&elseStatement)
    : Statement(sourceSpan), condition(std::move(condition)),
      thenBlock(std::move(thenBlock)),
      maybeElseStatement(std::move(elseStatement)) {}

void IfStatement::accept(AbstractASTVisitor &visitor) {
    visitor.visitIfStatement(*this);
}

std::vector<Node *> IfStatement::getChildren() const {
    std::vector<Node *> v;
    v.push_back(condition.get());
    v.push_back(thenBlock.get());
    if (maybeElseStatement != nullptr) {
        v.push_back(maybeElseStatement.get());
    }
    return v;
}

} // namespace Ceres::AST