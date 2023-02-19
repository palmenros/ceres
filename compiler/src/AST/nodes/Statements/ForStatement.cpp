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

#include "ForStatement.h"
#include "../../AbstractASTVisitor.h"

namespace Ceres::AST {
ForStatement::ForStatement(SourceSpan const& sourceSpan, std::unique_ptr<VariableDeclaration>&& maybeInitDeclaration,
    std::unique_ptr<Expression>&& maybeInitExpression, std::unique_ptr<Expression>&& conditionExpr,
    std::unique_ptr<Expression>&& updateExpr, std::unique_ptr<BlockStatement>&& body)
    : Statement(sourceSpan)
    , maybeInitDeclaration(std::move(maybeInitDeclaration))
    , maybeInitExpression(std::move(maybeInitExpression))
    , maybeConditionExpr(std::move(conditionExpr))
    , maybeUpdateExpr(std::move(updateExpr))
    , body(std::move(body))
{
}

void ForStatement::accept(AbstractASTVisitor& visitor) { visitor.visitForStatement(*this); }

std::vector<Node*> ForStatement::getChildren() const
{
    std::vector<Node*> v;

    if (maybeInitDeclaration != nullptr) {
        v.push_back(maybeInitDeclaration.get());
    }

    if (maybeInitExpression != nullptr) {
        v.push_back(maybeInitExpression.get());
    }

    if (maybeConditionExpr != nullptr) {
        v.push_back(maybeConditionExpr.get());
    }

    if (maybeUpdateExpr != nullptr) {
        v.push_back(maybeUpdateExpr.get());
    }

    v.push_back(body.get());

    return v;
}
} // namespace Ceres::AST