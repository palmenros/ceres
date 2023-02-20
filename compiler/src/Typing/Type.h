#ifndef COMPILER_TYPE_H
#define COMPILER_TYPE_H

#include "../utils/log.hpp"
#include "llvm/Support/Casting.h"
#include <string>
#include <string_view>
#include <unordered_map>

namespace Ceres {

namespace Typing {
class TypeVisitor;
}

// Note: Types are cached. There is only a unique instance per type. Types
// should have a private constructor and have a get() function that performs the
// cache lookup
class Type {
public:
    /// Discriminator for LLVM-style RTTI (dyn_cast<> et al.)
    // TODO: When adding a new type kind, you also have to add it here
    // TODO: When adding a new type kind, add it *at the end* and update the
    // coerced types table
    enum class TypeKind {
        UnitVoidType = 0,
        UnresolvedType,
        NotYetInferredType,
        PrimitiveScalarType,
        FunctionType,

        // TODO: Error Type must be the last tye
        ErrorType // This type represents an error type (for example, an invalid
                  // type given by the user)
    };

private:
    const TypeKind kind;

public:
    explicit Type(TypeKind kind)
        : kind(kind)
    {
    }
    [[nodiscard]] TypeKind getKind() const { return kind; }

    virtual void accept(Typing::TypeVisitor& visitor) = 0;
    [[nodiscard]] virtual std::string toString() const = 0;
    virtual ~Type() = default;

    static Type* getImplicitlyCoercedType(Type* a, Type* b);
};

class UnitVoidType : public Type {
private:
    static std::unique_ptr<UnitVoidType> instance;
    UnitVoidType()
        : Type(TypeKind::UnitVoidType)
    {
    }

public:
    // Static function needed for fast LLVM RTTI
    static bool classof(Type const* type) { return type->getKind() == TypeKind::UnitVoidType; }
    void accept(Typing::TypeVisitor& visitor) override;

    // Ours
    static UnitVoidType* get();
    [[nodiscard]] std::string toString() const override;
};

class ErrorType : public Type {
private:
    static std::unique_ptr<ErrorType> instance;
    ErrorType()
        : Type(TypeKind::ErrorType)
    {
    }

public:
    // Static function needed for fast LLVM RTTI
    static bool classof(Type const* type) { return type->getKind() == TypeKind::ErrorType; }
    void accept(Typing::TypeVisitor& visitor) override;

    // Ours
    static ErrorType* get();
    [[nodiscard]] std::string toString() const override;
};

// UnresolvedType is a type that hasn't been resolved yet. For example, it is a
// user-created type
//  and we haven't run yet type-binding, so we don't have fully resolved this
//  type.
class UnresolvedType : public Type {
private:
    static std::unordered_map<std::string, std::unique_ptr<UnresolvedType>> instances;
    explicit UnresolvedType(std::string typeIdentifier);

public:
    // Static function needed for fast LLVM RTTI
    static bool classof(Type const* type) { return type->getKind() == TypeKind::UnresolvedType; }
    void accept(Typing::TypeVisitor& visitor) override;

    // Ours
    std::string typeIdentifier;

    // TODO: Consider using std::string_view instead of std::string here and
    // everywhere else
    //          where it is appropriate.
    static UnresolvedType* get(std::string const& str);
    [[nodiscard]] std::string toString() const override;
};

// NotYetInferredKind is a type that hasn't been inferred yet and has not been
// specified by the user Examples:
//      Number literal without a specified type
//      Variable declaration without a user-specified type
enum class NotYetInferredKind {
    NumberLiteral,       // Number literal without a specified type such as '2'
                         // instead of '2u32'
    VariableDeclaration, // Variable declaration without a specified type such
                         // as 'const i = 1u32';
    Expression,          //
};

class NotYetInferredType : public Type {
private:
    static std::unordered_map<NotYetInferredKind, std::unique_ptr<NotYetInferredType>> instances;
    explicit NotYetInferredType(NotYetInferredKind kind)
        : kind(kind)
        , Type(TypeKind::NotYetInferredType)
    {
    }

public:
    // Static function needed for fast LLVM RTTI
    static bool classof(Type const* type) { return type->getKind() == TypeKind::NotYetInferredType; }
    void accept(Typing::TypeVisitor& visitor) override;

    // Ours
    NotYetInferredKind kind;

    static NotYetInferredType* get(NotYetInferredKind kind);
    [[nodiscard]] std::string toString() const override;
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
    // Bool
    BOOL,
};

class PrimitiveScalarType : public Type {
private:
    static std::unordered_map<PrimitiveKind, std::unique_ptr<PrimitiveScalarType>> instances;
    explicit PrimitiveScalarType(PrimitiveKind kind);

public:
    // Static function needed for fast LLVM RTTI
    static bool classof(Type const* type) { return type->getKind() == TypeKind::PrimitiveScalarType; }
    void accept(Typing::TypeVisitor& visitor) override;

    // Ours
    PrimitiveKind kind;

    static PrimitiveKind primitiveKindFromString(std::string_view str);

    static PrimitiveScalarType* get(PrimitiveKind kind);
    static PrimitiveScalarType* get(std::string_view str);

    static bool isBool(PrimitiveScalarType* const type)
    {
        switch (type->kind) {
        case PrimitiveKind::I8:
        case PrimitiveKind::I16:
        case PrimitiveKind::I32:
        case PrimitiveKind::I64:
        case PrimitiveKind::U8:
        case PrimitiveKind::U16:
        case PrimitiveKind::U32:
        case PrimitiveKind::U64:
        case PrimitiveKind::F32:
        case PrimitiveKind::F64:
            return false;
        case PrimitiveKind::BOOL:
            return true;
        default:
            NOT_IMPLEMENTED();
        }
    }

    static bool isInteger(PrimitiveScalarType* const type)
    {
        switch (type->kind) {
        case PrimitiveKind::I8:
        case PrimitiveKind::I16:
        case PrimitiveKind::I32:
        case PrimitiveKind::I64:
        case PrimitiveKind::U8:
        case PrimitiveKind::U16:
        case PrimitiveKind::U32:
        case PrimitiveKind::U64:
            return true;
        case PrimitiveKind::F32:
        case PrimitiveKind::F64:
        case PrimitiveKind::BOOL:
            return false;
        default:
            NOT_IMPLEMENTED();
        }
    }

    static bool isFloating(PrimitiveScalarType* const type)
    {
        switch (type->kind) {
        case PrimitiveKind::I8:
        case PrimitiveKind::I16:
        case PrimitiveKind::I32:
        case PrimitiveKind::I64:
        case PrimitiveKind::U8:
        case PrimitiveKind::U16:
        case PrimitiveKind::U32:
        case PrimitiveKind::U64:
        case PrimitiveKind::BOOL:
            return false;
        case PrimitiveKind::F32:
        case PrimitiveKind::F64:
            return true;
        default:
            NOT_IMPLEMENTED();
        }
    }

    [[nodiscard]] std::string toString() const override;
};

struct FunctionSignatureHash {
    size_t operator()(std::pair<Type*, std::vector<Type*>> const& p) const;
};

class FunctionType : public Type {
private:
    static std::unordered_map<std::pair<Type*, std::vector<Type*>>, std::unique_ptr<FunctionType>,
        FunctionSignatureHash>
        instances;

    explicit FunctionType(Type* returnType, std::vector<Type*> argumentTypes);

public:
    // Static function needed for fast LLVM RTTI
    static bool classof(Type const* type) { return type->getKind() == TypeKind::FunctionType; }

    void accept(Typing::TypeVisitor& visitor) override;

    // Ours
    Type* returnType;
    std::vector<Type*> argumentTypes;

    static FunctionType* get(Type* returnType, std::vector<Type*> const& argumentTypes);

    [[nodiscard]] std::string toString() const override;
};

} // namespace Ceres

#endif // COMPILER_TYPE_H
