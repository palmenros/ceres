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

#ifndef COMPILER_FLOATLITERALEXPRESSION_H
#define COMPILER_FLOATLITERALEXPRESSION_H

#include <string>
#include "Expression.h"

namespace Ceres::AST {

        enum class FloatLiteralBase {
            Dec,
            Hex
        };

        enum class FloatLiteralType {
            None,
            F32,
            F64
        };

        class FloatLiteralExpression : public Expression {
        public:
            FloatLiteralBase base;

            FloatLiteralType type;

            // String containing the integer literal in base "base" without the introducer characters for the base
            // and the type. For example: 0xfc783u32 -> fc783
            std::string str;

            FloatLiteralExpression(const SourceSpan &sourceSpan, FloatLiteralBase base, FloatLiteralType type, std::string str);

            static FloatLiteralType stringToFloatLiteralType(const std::string& str);

            void accept(AbstractASTVisitor &visitor) override;

            std::vector<Node *> getChildren() const override;
        };

    } // AST

#endif //COMPILER_FLOATLITERALEXPRESSION_H
