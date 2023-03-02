#include "Type.h"
#include "../utils/InitCeres.h"
#include "TypeVisitor.h"
#include "llvm/IR/DerivedTypes.h"
#include <cstdint>
#include <utility>

namespace Ceres {

std::unique_ptr<VoidType> VoidType::instance = nullptr;
std::unordered_map<std::string, std::unique_ptr<UnresolvedType>> UnresolvedType::instances;
std::unordered_map<NotYetInferredKind, std::unique_ptr<NotYetInferredType>> NotYetInferredType::instances;
std::unordered_map<PrimitiveIntegerKind, std::unique_ptr<PrimitiveIntegerType>> PrimitiveIntegerType::instances;
std::unordered_map<PrimitiveFloatKind, std::unique_ptr<PrimitiveFloatType>> PrimitiveFloatType::instances;
std::unordered_map<std::pair<Type*, std::vector<Type*>>, std::unique_ptr<FunctionType>, FunctionSignatureHash>
    FunctionType::instances;
std::unique_ptr<ErrorType> ErrorType::instance = nullptr;
std::unique_ptr<BoolType> BoolType::instance = nullptr;

std::string VoidType::toString() const { return "void"; }

VoidType* VoidType::get()
{
    if (instance != nullptr) {
        return instance.get();
    }

    instance.reset(new VoidType);
    return instance.get();
}

void VoidType::accept(Typing::TypeVisitor& visitor) { visitor.visitUnitVoidType(this); }

llvm::Type* VoidType::doGetLLVMType() const { return llvm::Type::getVoidTy(*InitCeres::getLLVMContext()); }

PrimitiveIntegerType::PrimitiveIntegerType(PrimitiveIntegerKind kind)
    : kind(kind)
    , Type(TypeKind::PrimitiveIntegerType)
{
}

PrimitiveIntegerKind PrimitiveIntegerType::primitiveIntegerKindFromString(std::string_view str)
{
    // TODO: Maybe change to a lookup into a static hashmap?
    if (str == "i8") {
        return PrimitiveIntegerKind::I8;
    } else if (str == "u8") {
        return PrimitiveIntegerKind::U8;
    } else if (str == "i16") {
        return PrimitiveIntegerKind::I16;
    } else if (str == "u16") {
        return PrimitiveIntegerKind::U16;
    } else if (str == "i32") {
        return PrimitiveIntegerKind::I32;
    } else if (str == "u32") {
        return PrimitiveIntegerKind::U32;
    } else if (str == "i64") {
        return PrimitiveIntegerKind::I64;
    } else if (str == "u64") {
        return PrimitiveIntegerKind::U64;
    } else {
        Log::panic("Unknown primitive integer type: {}", str);
    }
}

std::string PrimitiveIntegerType::toString() const
{
    switch (kind) {
    case PrimitiveIntegerKind::I8:
        return "i8";
    case PrimitiveIntegerKind::U8:
        return "u8";
    case PrimitiveIntegerKind::I16:
        return "i16";
    case PrimitiveIntegerKind::U16:
        return "u16";
    case PrimitiveIntegerKind::I32:
        return "i32";
    case PrimitiveIntegerKind::U32:
        return "u32";
    case PrimitiveIntegerKind::I64:
        return "i64";
    case PrimitiveIntegerKind::U64:
        return "u64";
    default:
        NOT_IMPLEMENTED();
    }
}

PrimitiveIntegerType* PrimitiveIntegerType::get(std::string_view str)
{
    return get(primitiveIntegerKindFromString(str));
}

PrimitiveIntegerType* PrimitiveIntegerType::get(PrimitiveIntegerKind kind)
{
    auto& ptr = instances[kind];
    if (ptr == nullptr) {
        ptr.reset(new PrimitiveIntegerType(kind));
    }
    return ptr.get();
}

void PrimitiveIntegerType::accept(Typing::TypeVisitor& visitor) { visitor.visitPrimitiveIntegerType(this); }

llvm::Type* PrimitiveIntegerType::doGetLLVMType() const
{
    // Note: LLVM does not have unsigned types, they are represented as signed integers too

    llvm::LLVMContext& ctx = *InitCeres::getLLVMContext();
    switch (kind) {
    case PrimitiveIntegerKind::I8:
    case PrimitiveIntegerKind::U8:
        return llvm::Type::getInt8Ty(ctx);
    case PrimitiveIntegerKind::U16:
    case PrimitiveIntegerKind::I16:
        return llvm::Type::getInt16Ty(ctx);
    case PrimitiveIntegerKind::U32:
    case PrimitiveIntegerKind::I32:
        return llvm::Type::getInt32Ty(ctx);
    case PrimitiveIntegerKind::I64:
    case PrimitiveIntegerKind::U64:
        return llvm::Type::getInt64Ty(ctx);
    default:
        NOT_IMPLEMENTED();
    }
}
unsigned int PrimitiveIntegerType::getNumBits() const
{
    switch (kind) {
    case PrimitiveIntegerKind::I8:
    case PrimitiveIntegerKind::U8:
        return 8;
    case PrimitiveIntegerKind::I16:
    case PrimitiveIntegerKind::U16:
        return 16;
    case PrimitiveIntegerKind::I32:
    case PrimitiveIntegerKind::U32:
        return 32;
    case PrimitiveIntegerKind::I64:
    case PrimitiveIntegerKind::U64:
        return 64;
    default:
        NOT_IMPLEMENTED();
    }
}
bool PrimitiveIntegerType::isSigned() const
{
    switch (kind) {
    case PrimitiveIntegerKind::I8:
    case PrimitiveIntegerKind::I16:
    case PrimitiveIntegerKind::I32:
    case PrimitiveIntegerKind::I64:
        return true;
    case PrimitiveIntegerKind::U8:
    case PrimitiveIntegerKind::U16:
    case PrimitiveIntegerKind::U32:
    case PrimitiveIntegerKind::U64:
        return false;
    default:
        NOT_IMPLEMENTED();
    }
}

std::string NotYetInferredType::toString() const
{
    switch (kind) {
    case NotYetInferredKind::IntegerLiteral:
        return "integer literal";
    case NotYetInferredKind::FloatLiteral:
        return "float literal";
    case NotYetInferredKind::VariableDeclaration:
        return "$UnresolvedVariableDeclaration";
    case NotYetInferredKind::Expression:
        return "$UnresolvedExpression";
    default:
        NOT_IMPLEMENTED();
    }
}

NotYetInferredType* NotYetInferredType::get(NotYetInferredKind kind)
{
    auto& ptr = instances[kind];
    if (ptr == nullptr) {
        ptr.reset(new NotYetInferredType(kind));
    }
    return ptr.get();
}

void NotYetInferredType::accept(Typing::TypeVisitor& visitor) { visitor.visitNotYetInferredType(this); }

std::optional<Type*> NotYetInferredType::getDefaultType() const
{
    switch (kind) {
    case NotYetInferredKind::IntegerLiteral:
        return PrimitiveIntegerType::get(PrimitiveIntegerKind::I32);
    case NotYetInferredKind::FloatLiteral:
        return PrimitiveFloatType::get(PrimitiveFloatKind::F32);
    case NotYetInferredKind::VariableDeclaration:
    case NotYetInferredKind::Expression:
    default:
        return {};
    }
}
llvm::Type* NotYetInferredType::doGetLLVMType() const { ASSERT_NOT_REACHED(); }

UnresolvedType::UnresolvedType(std::string typeIdentifier)
    : typeIdentifier(std::move(typeIdentifier))
    , Type(TypeKind::UnresolvedType)
{
}

std::string UnresolvedType::toString() const { return typeIdentifier; }

UnresolvedType* UnresolvedType::get(std::string const& str)
{
    auto& ptr = instances[str];
    if (ptr == nullptr) {
        ptr.reset(new UnresolvedType(str));
    }
    return ptr.get();
}

void UnresolvedType::accept(Typing::TypeVisitor& visitor) { visitor.visitUnresolvedType(this); }

llvm::Type* UnresolvedType::doGetLLVMType() const { ASSERT_NOT_REACHED(); }

void FunctionType::accept(Typing::TypeVisitor& visitor) { visitor.visitFunctionType(this); }

FunctionType::FunctionType(Type* returnType, std::vector<Type*> argumentTypes)
    : returnType(returnType)
    , argumentTypes(std::move(argumentTypes))
    , Type(TypeKind::FunctionType)
{
}

FunctionType* FunctionType::get(Type* returnType, std::vector<Type*> const& argumentTypes)
{
    auto& ptr = instances[std::make_pair(returnType, argumentTypes)];
    if (ptr == nullptr) {
        ptr.reset(new FunctionType(returnType, argumentTypes));
    }
    return ptr.get();
}

std::string FunctionType::toString() const
{
    std::string arguments;
    bool first = true;
    for (auto arg : argumentTypes) {
        if (first) {
            first = false;
        } else {
            arguments += ", ";
        }
        arguments += arg->toString();
    }

    return fmt::format("fn ({}) {}", arguments, returnType->toString());
}

llvm::Type* FunctionType::doGetLLVMType() const
{
    std::vector<llvm::Type*> argumentLLVMTypes;
    std::transform(argumentTypes.begin(), argumentTypes.end(), std::back_inserter(argumentLLVMTypes),
        [](Type* type) { return type->getLLVMType(); });

    // TODO: Update when supporting vararg
    return llvm::FunctionType::get(returnType->getLLVMType(), argumentLLVMTypes, false);
}

template<class T> inline void hash_combine(std::size_t& seed, T const& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

size_t FunctionSignatureHash::operator()(std::pair<Type*, std::vector<Type*>> const& p) const
{
    auto const& vec = p.second;
    Type* t = p.first;

    size_t seed = vec.size();
    hash_combine(seed, (uintptr_t)t);
    for (auto ptr : vec) {
        auto x = (uintptr_t)ptr;
        x = ((x >> 16) ^ x) * 0x45d9f3b;
        x = ((x >> 16) ^ x) * 0x45d9f3b;
        x = (x >> 16) ^ x;
        seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
}

void ErrorType::accept(Typing::TypeVisitor& visitor) { visitor.visitErrorType(this); }

ErrorType* ErrorType::get()
{
    if (instance != nullptr) {
        return instance.get();
    }

    instance.reset(new ErrorType);
    return instance.get();
}

std::string ErrorType::toString() const { return "<ErrorType>"; }

llvm::Type* ErrorType::doGetLLVMType() const { ASSERT_NOT_REACHED(); }

// Return ErrorType if the coercion is not possible
Type* Type::getImplicitlyCoercedType(Type* a, Type* b)
{
    // If same coercion is obvious
    if (a == b) {
        return b;
    }

    // a will hold the type with the *smallest* typeKind
    if (b->getKind() < a->getKind()) {
        std::swap(a, b);
    }

    switch (a->getKind()) {
    case TypeKind::UnitVoidType: {
        return ErrorType::get();
    }
    case TypeKind::UnresolvedType: {
        Log::panic("Unresolved type when coercing");
    }
    case TypeKind::NotYetInferredType: {
        auto* notYetInferredType = llvm::dyn_cast<NotYetInferredType>(a);
        ASSERT(notYetInferredType != nullptr);

        // Handle coercion between literal types and integers
        if (llvm::isa<PrimitiveIntegerType>(b)) {
            if (notYetInferredType->kind == NotYetInferredKind::IntegerLiteral) {
                return b;
            }
        } else if (llvm::isa<PrimitiveFloatType>(b)) {
            if (notYetInferredType->kind == NotYetInferredKind::FloatLiteral) {
                return b;
            }
        }
        return ErrorType::get();
    }
    case TypeKind::PrimitiveIntegerType: {
        // TODO: If we allow for implicit safe scalar type coercion u8 -> u32,
        // this is
        //          where to add it.
        return ErrorType::get();
    }
    case TypeKind::PrimitiveFloatType: {
        // TODO: If we allow for implicit safe scalar type coercion f32 -> f64,
        // this is
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

llvm::Type* Type::getLLVMType() const
{
    if (llvmType == nullptr) {
        llvmType = doGetLLVMType();
    }
    return llvmType;
}

BoolType::BoolType()
    : Type(TypeKind::Bool)
{
}

std::string BoolType::toString() const { return "bool"; }

BoolType* BoolType::get()
{
    if (instance != nullptr) {
        return instance.get();
    }

    instance.reset(new BoolType);
    return instance.get();
}

void BoolType::accept(Typing::TypeVisitor& visitor) { visitor.visitBoolType(this); }

llvm::Type* BoolType::doGetLLVMType() const { return llvm::Type::getInt1Ty(*InitCeres::getLLVMContext()); }

PrimitiveFloatType::PrimitiveFloatType(PrimitiveFloatKind kind)
    : Type(TypeKind::PrimitiveFloatType)
    , kind(kind)
{
}
void PrimitiveFloatType::accept(Typing::TypeVisitor& visitor) { visitor.visitPrimitiveFloatType(this); }

std::string PrimitiveFloatType::toString() const
{
    switch (kind) {
    case PrimitiveFloatKind::F32:
        return "f32";
    case PrimitiveFloatKind::F64:
        return "f64";
    default:
        NOT_IMPLEMENTED();
    }
}
PrimitiveFloatType* PrimitiveFloatType::get(PrimitiveFloatKind kind)
{
    auto& ptr = instances[kind];
    if (ptr == nullptr) {
        ptr.reset(new PrimitiveFloatType(kind));
    }
    return ptr.get();
}

PrimitiveFloatType* PrimitiveFloatType::get(std::string_view str)
{
    return PrimitiveFloatType::get(primitiveFloatKindFromString(str));
}

PrimitiveFloatKind PrimitiveFloatType::primitiveFloatKindFromString(std::string_view str)
{
    if (str == "f32") {
        return PrimitiveFloatKind::F32;
    } else if (str == "f64") {
        return PrimitiveFloatKind::F64;
    } else {
        Log::panic("Primitive Float Kind Not implemented");
    }
}

llvm::Type* PrimitiveFloatType::doGetLLVMType() const
{
    llvm::LLVMContext& ctx = *InitCeres::getLLVMContext();
    switch (kind) {
    case PrimitiveFloatKind::F32:
        return llvm::Type::getFloatTy(ctx);
    case PrimitiveFloatKind::F64:
        return llvm::Type::getDoubleTy(ctx);
    default:
        NOT_IMPLEMENTED();
    }
}

} // namespace Ceres