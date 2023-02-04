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

#ifndef COMPILER_INTLITERAL_H
#define COMPILER_INTLITERAL_H

#include <string>
#include "Expression.h"

namespace Ceres {
    namespace AST {

        enum class IntLiteralBase {
            Dec,
            Hex,
            Oct,
            Bin
        };

        enum class IntLiteralType {
            None,
            U8,
            U16,
            U32,
            U64,
            I8,
            I16,
            I32,
            I64
        };

        class IntLiteral : public Expression {
        public:

            IntLiteralBase base;

            IntLiteralType type;

            // String containing the integer literal in base "base" without the introducer characters for the base
            // and the type. For example: 0xfc783u32 -> fc783
            std::string str;

            IntLiteral(const SourceSpan &sourceSpan, IntLiteralBase base, IntLiteralType type, const std::string &str);

            static IntLiteralType stringToIntLiteralType(const std::string& str);

        };

    } // Ceres
} // AST

#endif //COMPILER_INTLITERAL_H
