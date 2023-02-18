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

#include <stdint.h>
#include <utility>

namespace Ceres {

    std::unique_ptr<UnitVoidType> UnitVoidType::instance = nullptr;
    std::unordered_map<std::string, std::unique_ptr<UnresolvedType>> UnresolvedType::instances;
    std::unordered_map<NotYetInferredKind, std::unique_ptr<NotYetInferredType>>
            NotYetInferredType::instances;
    std::unordered_map<PrimitiveKind, std::unique_ptr<PrimitiveScalarType>>
            PrimitiveScalarType::instances;
    std::unordered_map<std::pair<Type *, std::vector<Type *>>, std::unique_ptr<FunctionType>,
                       FunctionSignatureHash>
            FunctionType::instances;
    std::unique_ptr<ErrorType> ErrorType::instance = nullptr;

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
            case NotYetInferredKind::Expression:
                return "$UnresolvedExpression";
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

    void FunctionType::accept(Typing::TypeVisitor &visitor) { visitor.visitFunctionType(this); }

    FunctionType::FunctionType(Type *returnType, std::vector<Type *> argumentTypes)
        : returnType(returnType), argumentTypes(std::move(argumentTypes)),
          Type(TypeKind::FunctionType) {}


    FunctionType *FunctionType::get(Type *returnType, const std::vector<Type *> &argumentTypes) {
        auto &ptr = instances[std::make_pair(returnType, argumentTypes)];
        if (ptr == nullptr) { ptr.reset(new FunctionType(returnType, argumentTypes)); }
        return ptr.get();
    }

    std::string FunctionType::toString() const {
        std::string arguments;
        bool first = true;
        for (auto arg: argumentTypes) {
            if (first) {
                first = false;
            } else {
                arguments += ", ";
            }
            arguments += arg->toString();
        }

        return fmt::format("fn ({}) {}", arguments, returnType->toString());
    }

    template<class T>
    inline void hash_combine(std::size_t &seed, const T &v) {
        std::hash<T> hasher;
        seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    size_t
    FunctionSignatureHash::operator()(const std::pair<Type *, std::vector<Type *>> &p) const {
        const auto &vec = p.second;
        Type *t = p.first;

        size_t seed = vec.size();
        hash_combine(seed, (uintptr_t) t);
        for (auto ptr: vec) {
            auto x = (uintptr_t) ptr;
            x = ((x >> 16) ^ x) * 0x45d9f3b;
            x = ((x >> 16) ^ x) * 0x45d9f3b;
            x = (x >> 16) ^ x;
            seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }

    void ErrorType::accept(Typing::TypeVisitor &visitor) { visitor.visitErrorType(this); }

    ErrorType *ErrorType::get() {
        if (instance != nullptr) { return instance.get(); }

        instance.reset(new ErrorType);
        return instance.get();
    }

    std::string ErrorType::toString() const { return "<ErrorType>"; }

    // Return ErrorType if the coercion is not pssible
    Type *Type::getImplicitlyCoercedType(Type *a, Type *b) {
        // If same coercion is obvious
        if (a == b) { return b; }

        // a will hold the type with the *smallest* typeKind
        if (b->getKind() < a->getKind()) { std::swap(a, b); }

        switch (a->getKind()) {
            case TypeKind::UnitVoidType: {
                return ErrorType::get();
            }
            case TypeKind::UnresolvedType: {
                Log::panic("Unresolved type when coercing");
            }
            case TypeKind::NotYetInferredType: {
                auto *notYetInferredType = llvm::dyn_cast<NotYetInferredType>(a);
                ASSERT(notYetInferredType != nullptr);
                if (auto *primitiveScalarType = llvm::dyn_cast<PrimitiveScalarType>(b)) {
                    if (notYetInferredType->kind == NotYetInferredKind::NumberLiteral) { return b; }
                }
                return ErrorType::get();
            }
            case TypeKind::PrimitiveScalarType: {
                // TODO: If we allow for implicit safe scalar type coercion u8 -> u32, this is
                //          where to add it.
                return ErrorType::get();
            }
            case TypeKind::FunctionType: {
                return ErrorType::get();
            }
            case TypeKind::ErrorType: {
                return ErrorType::get();
            }
            default:
                NOT_IMPLEMENTED();
        }

        NOT_IMPLEMENTED();
    }


}// namespace Ceres