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
#include <unordered_map>

namespace Ceres {
    class Type {
    public:
        virtual std::string toString() const = 0;
        virtual ~Type() = default;
    };

    class UnitVoidType : public Type {
    private:
        static std::unique_ptr<UnitVoidType> instance;
        UnitVoidType() = default;

    public:
        static UnitVoidType *get();

        std::string toString() const override;
    };

    // UnresolvedType is a type that hasn't been resolved yet. For example, it is a user-created type
    //  and we haven't run yet type-binding, so we don't have fully resolved this type.
    class UnresolvedType : public Type {
    private:
        static std::unordered_map<std::string, std::unique_ptr<UnresolvedType>> instances;
        explicit UnresolvedType(std::string typeIdentifier);

    public:
        std::string typeIdentifier;

        static UnresolvedType *get(const std::string &str);

        std::string toString() const override;
    };

    // NotYetInferredKind is a type that hasn't been inferred yet and has not been specified by the user
    // Examples:
    //      Number literal without a specified type
    //      Variable declaration without a user-specified type
    enum class NotYetInferredKind {
        NumberLiteral,     // Number literal without a specified type such as '2' instead of '2u32'
        VariableDeclaration// Variable declaration without a specified type such as 'const i = 1u32';
    };

    class NotYetInferredType : public Type {
    private:
        static std::unordered_map<NotYetInferredKind, std::unique_ptr<NotYetInferredType>> instances;
        explicit NotYetInferredType(NotYetInferredKind kind) : kind(kind) {}

    public:
        NotYetInferredKind kind;

        static NotYetInferredType *get(NotYetInferredKind kind);
        std::string toString() const override;
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

    class PrimitiveScalarType : public Type {
    private:
        static std::unordered_map<PrimitiveKind, std::unique_ptr<PrimitiveScalarType>> instances;
        explicit PrimitiveScalarType(PrimitiveKind kind);

    public:
        PrimitiveKind kind;

        static PrimitiveKind primitiveKindFromString(const std::string &str);

        static PrimitiveScalarType *get(PrimitiveKind kind);
        static PrimitiveScalarType *get(const std::string &str);
        std::string toString() const override;
    };


}// namespace Ceres

#endif//COMPILER_TYPE_H
