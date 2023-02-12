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

#ifndef COMPILER_PREFIXEXPRESSION_H
#define COMPILER_PREFIXEXPRESSION_H

#include "Expression.h"
#include <memory>

namespace Ceres::AST {

    enum class PrefixOp {
        PrefixIncrement,
        PrefixDecrement,
        UnaryPlus,
        UnaryMinus,
        UnaryLogicalNot,
        UnaryBitwiseNot
    };

    class PrefixExpression : public Expression {
    public:
        PrefixOp op;
        std::unique_ptr<Expression> expr;

        SourceSpan opSourceSpan;

        PrefixExpression(const SourceSpan &sourceSpan, PrefixOp op,
                         std::unique_ptr<Expression> &&expr, const SourceSpan &opSourceSpan);

        void accept(AbstractASTVisitor &visitor) override;

        std::vector<Node *> getChildren() const override;
    };

}// namespace Ceres::AST

#endif//COMPILER_PREFIXEXPRESSION_H
