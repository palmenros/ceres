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
    virtual SymbolDeclaration resolve(std::string const& name) = 0;
};

class SymbolTableScope : public Scope {
private:
    std::unordered_map<std::string, SymbolDeclaration> map;

public:
    explicit SymbolTableScope(Scope* enclosingScope);

    void define(std::string const& name, SymbolDeclaration const& symbol) override;

    SymbolDeclaration resolve(std::string const& name) override;
};
} // namespace Ceres::Binding

#endif // COMPILER_SCOPE_H
