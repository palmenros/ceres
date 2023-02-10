/*
 * Copyright (C) 2023 Pedro Palacios Almendros, Ricardo Maurizio Paul
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

#include "Type.h"
#include "TypeVisitor.h"

#include <utility>

namespace Ceres {

    std::unique_ptr<UnitVoidType> UnitVoidType::instance = nullptr;
    std::unordered_map<std::string, std::unique_ptr<UnresolvedType>> UnresolvedType::instances;
    std::unordered_map<NotYetInferredKind, std::unique_ptr<NotYetInferredType>>
            NotYetInferredType::instances;
    std::unordered_map<PrimitiveKind, std::unique_ptr<PrimitiveScalarType>>
            PrimitiveScalarType::instances;

    std::string UnitVoidType::toString() const { return "void"; }

    UnitVoidType *UnitVoidType::get() {
        if (instance != nullptr) { return instance.get(); }

        instance.reset(new UnitVoidType);
        return instance.get();
    }

    void UnitVoidType::accept(Typing::TypeVisitor &visitor) { visitor.visitUnitVoidType(this); }

    PrimitiveScalarType::PrimitiveScalarType(PrimitiveKind kind)
        : kind(kind), Type(TypeKind::PrimitiveScalarType) {}

    PrimitiveKind PrimitiveScalarType::primitiveKindFromString(std::string_view str) {
        // TODO: Maybe change to a lookup into a static hashmap?
        if (str == "i8") {
            return PrimitiveKind::I8;
        } else if (str == "u8") {
            return PrimitiveKind::U8;
        } else if (str == "i16") {
            return PrimitiveKind::I16;
        } else if (str == "u16") {
            return PrimitiveKind::U16;
        } else if (str == "i32") {
            return PrimitiveKind::I32;
        } else if (str == "u32") {
            return PrimitiveKind::U32;
        } else if (str == "i64") {
            return PrimitiveKind::I64;
        } else if (str == "u64") {
            return PrimitiveKind::U64;
        } else if (str == "f32") {
            return PrimitiveKind::F32;
        } else if (str == "f64") {
            return PrimitiveKind::F64;
        } else {
            Log::panic("Unknown primitive type: {}", str);
        }
    }

    std::string PrimitiveScalarType::toString() const {
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
                NOT_IMPLEMENTED();
        }
    }

    PrimitiveScalarType *PrimitiveScalarType::get(std::string_view str) {
        return get(primitiveKindFromString(str));
    }

    PrimitiveScalarType *PrimitiveScalarType::get(PrimitiveKind kind) {
        auto &ptr = instances[kind];
        if (ptr == nullptr) { ptr.reset(new PrimitiveScalarType(kind)); }
        return ptr.get();
    }

    void PrimitiveScalarType::accept(Typing::TypeVisitor &visitor) {
        visitor.visitPrimitiveScalarType(this);
    }

    std::string NotYetInferredType::toString() const {
        switch (kind) {
            case NotYetInferredKind::NumberLiteral:
                return "$UnresolvedNumberLiteral";
            case NotYetInferredKind::VariableDeclaration:
                return "$UnresolvedVariableDeclaration";
            default:
                NOT_IMPLEMENTED();
        }
    }

    NotYetInferredType *NotYetInferredType::get(NotYetInferredKind kind) {
        auto &ptr = instances[kind];
        if (ptr == nullptr) { ptr.reset(new NotYetInferredType(kind)); }
        return ptr.get();
    }

    void NotYetInferredType::accept(Typing::TypeVisitor &visitor) {
        visitor.visitNotYetInferredType(this);
    }

    UnresolvedType::UnresolvedType(std::string typeIdentifier)
        : typeIdentifier(std::move(typeIdentifier)), Type(TypeKind::UnresolvedType) {}

    std::string UnresolvedType::toString() const { return typeIdentifier; }

    UnresolvedType *UnresolvedType::get(const std::string &str) {
        auto &ptr = instances[str];
        if (ptr == nullptr) { ptr.reset(new UnresolvedType(str)); }
        return ptr.get();
    }

    void UnresolvedType::accept(Typing::TypeVisitor &visitor) { visitor.visitUnresolvedType(this); }
}// namespace Ceres