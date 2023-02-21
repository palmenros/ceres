#ifndef COMPILER_SCOPE_H
#define COMPILER_SCOPE_H

#include "SymbolDeclaration.h"
#include <string>
#include <unordered_map>
#include <utility>

namespace Ceres::Binding {
// Abstract class representing a scope
class Scope {
protected:
    Scope* enclosingScope;
    explicit Scope(Scope* enclosingScope);

public:
    Scope* getEnclosingScope();

    virtual void define(std::string const& name, SymbolDeclaration const& symbol) = 0;
    virtual std::optional<SymbolDeclaration> resolve(std::string const& name) = 0;
};

class SymbolTableScope : public Scope {
private:
    std::unordered_map<std::string, SymbolDeclaration> map;

public:
    explicit SymbolTableScope(Scope* enclosingScope);
    void define(std::string const& name, SymbolDeclaration const& symbol) override;
    std::optional<SymbolDeclaration> resolve(std::string const& name) override;
};
} // namespace Ceres::Binding

#endif // COMPILER_SCOPE_H
