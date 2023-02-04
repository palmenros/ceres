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

#include "IntLiteralExpression.h"
#include <unordered_map>

namespace Ceres::AST {

        IntLiteralExpression::IntLiteralExpression(const SourceSpan &sourceSpan, IntLiteralBase base, IntLiteralType type,
                                                   std::string str) : Expression(sourceSpan), base(base), type(type), str(std::move(str)) {}

        IntLiteralType IntLiteralExpression::stringToIntLiteralType(const std::string & str) {

            static const std::unordered_map<std::string, IntLiteralType> intLiteralStrings {
                    { "u8", IntLiteralType::U8 },
                    { "u16", IntLiteralType::U16 },
                    { "u32", IntLiteralType::U32 },
                    { "u64", IntLiteralType::U64 },
                    { "i8", IntLiteralType::I8 },
                    { "i16", IntLiteralType::I16 },
                    { "i32", IntLiteralType::I32 },
                    { "i64", IntLiteralType::I64 },
            };

            auto it = intLiteralStrings.find(str);
            if(it == intLiteralStrings.end()) {
                return IntLiteralType::None;
            }

            return it->second;
        }
    } // AST