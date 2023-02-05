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

#ifndef COMPILER_BINARYOPERATIONEXPRESSION_H
#define COMPILER_BINARYOPERATIONEXPRESSION_H

#include <memory>
#include "Expression.h"

namespace Ceres::AST {

    enum class BinaryOp {
        Mult,
        Div,
        Modulo,
        Sum,
        Subtraction,
        BitshiftLeft,
        BitshiftRight,
        LessOrEqual,
        GreaterOrEqual,
        GreaterThan,
        LessThan,
        BitwiseAnd,
        BitwiseOr,
        BitwiseXor,
        Equals,
        NotEquals,
        LogicalAnd,
        LogicalOr
    };

    class BinaryOperationExpression : public Expression {
    public:

        std::unique_ptr<Expression> left, right;
        BinaryOp op;

        SourceSpan opSpan;

        BinaryOperationExpression(const SourceSpan &sourceSpan,
                                  std::unique_ptr<Expression> &&left,
                                  std::unique_ptr<Expression> &&right, BinaryOp op,
                                  SourceSpan opSpan);

        void accept(AbstractASTVisitor &visitor) override;

        std::vector<Node *> getChildren() const override;
    };

} // AST

#endif //COMPILER_BINARYOPERATIONEXPRESSION_H