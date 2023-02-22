#include "Scope.h"
#include "../Diagnostics/Diagnostics.h"

namespace Ceres::Binding {
Scope::Scope(Scope* enclosingScope)
    : enclosingScope(enclosingScope) {};

Scope* Scope::getEnclosingScope() { return enclosingScope; };

SymbolTableScope::SymbolTableScope(Scope* enclosingScope)
    : Scope(enclosingScope) {};

void SymbolTableScope::define(std::string const& name, SymbolDeclaration const& symbol)
{
    auto [it, inserted_new] = map.insert(std::make_pair(name, symbol));
    if (!inserted_new) {
        // An element with that scopeName already existed
        Diagnostics::report(symbol.getDeclarationNode()->sourceSpan, Diag::duplicate_symbol, name);
        Diagnostics::report(it->second.getDeclarationNode()->sourceSpan, Diag::duplicate_symbol_note, name);
    }
};

std::optional<SymbolDeclaration> SymbolTableScope::resolve(std::string const& name)
{
    auto it = map.find(name);
    if (it != map.end()) {
        return it->second;
    }

    if (getEnclosingScope() == nullptr) {
        return {};
    }

    return getEnclosingScope()->resolve(name);
};

} // namespace Ceres::Binding