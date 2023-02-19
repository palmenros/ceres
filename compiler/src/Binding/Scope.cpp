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

#include "Scope.h"

namespace Ceres::Binding {
Scope::Scope(Scope *enclosingScope) : enclosingScope(enclosingScope){};

Scope *Scope::getEnclosingScope() { return enclosingScope; };

SymbolTableScope::SymbolTableScope(Scope *enclosingScope)
    : Scope(enclosingScope){};

void SymbolTableScope::define(const std::string &name,
                              const SymbolDeclaration &symbol) {
    auto [it, inserted_new] = map.insert(std::make_pair(name, symbol));
    if (!inserted_new) {
        // An element with that scopeName already existed
        Log::panic("duplicate symbol");
    }
};

SymbolDeclaration SymbolTableScope::resolve(const std::string &name) {
    auto it = map.find(name);
    if (it != map.end()) {
        return it->second;
    }

    if (getEnclosingScope() == nullptr) {
        Log::panic("undefined symbol");
    }

    return getEnclosingScope()->resolve(name);
};

} // namespace Ceres::Binding