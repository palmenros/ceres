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

#ifndef COMPILER_TYPE_H
#define COMPILER_TYPE_H

#include "../utils/log.hpp"
#include <string>

namespace Ceres::AST {
    class Type {
    protected:
        // Type name. Can be the empty string if no type has been provided.
        // std::string name;

        // Called by the AST generator to create a type with an specified (non-empty name)
        // explicit Type(std::string name);

        // Called by the AST generator to create a type when none has been given by the user
        // static Type createUnspecifiedType();

        virtual std::string toString();
    };

    class Unit : Type {
    public:
        std::string toString() override {
            return "Unit";
        }
    };

    enum class UnresolvedKind {
        NumberLiteral
    };

    class Unresolved : Type {
        UnresolvedKind kind;

        Unresolved(UnresolvedKind kind) : kind(kind) {}

        std::string toString() override {
            switch (kind) {
                case UnresolvedKind::NumberLiteral:
                    return "numberLit";
                default:
                    Log::panic("Wot???");
            }
        }
    };

    enum class PrimitiveKind {
        // signed
        I8,
        I16,
        I32,
        I64,
        // unsigned
        U8,
        U16,
        U32,
        U64,
        // float
        F32,
        F64,
    };

    class Primitive : Type {
    public:
        PrimitiveKind kind;

        Primitive(const std::string &str) {
            if (str == "i8") {
                kind = PrimitiveKind::I8;
            } else if (str == "u8") {
                kind = PrimitiveKind::U8;
            } else if (str == "i16") {
                kind = PrimitiveKind::I16;
            } else if (str == "u16") {
                kind = PrimitiveKind::U16;
            } else if (str == "i32") {
                kind = PrimitiveKind::I32;
            } else if (str == "u32") {
                kind = PrimitiveKind::U32;
            } else if (str == "i64") {
                kind = PrimitiveKind::I64;
            } else if (str == "u64") {
                kind = PrimitiveKind::U64;
            } else if (str == "f32") {
                kind = PrimitiveKind::F32;
            } else if (str == "f64") {
                kind = PrimitiveKind::F64;
            } else {
                Log::panic("Unknown type");
            }
        }

        std::string toString() override {
            switch (kind) {
                case PrimitiveKind::I8:
                    return "i8";
                case PrimitiveKind::U8:
                    return "u8";
                case PrimitiveKind::I16:
                    return "i16";
                case PrimitiveKind::U16:
                    return "u16";
                case PrimitiveKind::I32:
                    return "i32";
                case PrimitiveKind::U32:
                    return "u32";
                case PrimitiveKind::I64:
                    return "i64";
                case PrimitiveKind::U64:
                    return "u64";
                case PrimitiveKind::F32:
                    return "f32";
                case PrimitiveKind::F64:
                    return "f64";
                default:
                    Log::panic("Wot???");
            }
        }
    };


}// namespace Ceres::AST

#endif//COMPILER_TYPE_H
