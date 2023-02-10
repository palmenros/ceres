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

#ifndef COMPILER_FORSTATEMENT_H
#define COMPILER_FORSTATEMENT_H

#include "BlockStatement.h"
#include "Expression.h"
#include "Statement.h"
#include "VariableDeclaration.h"
#include <memory>

namespace Ceres::AST {

    class ForStatement : public Statement {
    public:
        // Note: Any expression in the for except the block can be a nullptr (for (;;;) {}).
        // moreover, For may have as its first argument either a declaration or an expression:
        // It may have a declaration (for(let i = 0;;){}) or an expression (let i : i32; for(i = 0;;){}).
        // At most one of them will be a non nullptr, but both may be nullptr.
        std::unique_ptr<VariableDeclaration> maybeInitDeclaration;
        std::unique_ptr<Expression> maybeInitExpression;

        std::unique_ptr<Expression> maybeConditionExpr;
        std::unique_ptr<Expression> maybeUpdateExpr;

        std::unique_ptr<BlockStatement> body;

        ForStatement(const SourceSpan &sourceSpan,
                     std::unique_ptr<VariableDeclaration> &&maybeInitDeclaration,
                     std::unique_ptr<Expression> &&maybeInitExpression,
                     std::unique_ptr<Expression> &&conditionExpr,
                     std::unique_ptr<Expression> &&updateExpr,
                     std::unique_ptr<BlockStatement> &&body);

        void accept(AbstractASTVisitor &visitor) override;

        std::vector<Node *> getChildren() const override;
    };

}// namespace Ceres::AST

#endif//COMPILER_FORSTATEMENT_H
