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

#ifndef COMPILER_SCOPE_H
#define COMPILER_SCOPE_H

#include "SymbolDeclaration.h"
#include <string>

namespace Ceres::Binding {

    // Abstract class representing a scope
    class Scope {
    public:
        virtual std::string getScopeName() = 0;
        virtual Scope *getEnclosingScope() = 0;
        virtual void define(const SymbolDeclaration &symbol) = 0;
        virtual SymbolDeclaration resolve(const std::string &name) = 0;
    };

    // TODO: Maybe GlobalScope can also be implemented as a LocalScope so rename it.
    //          We will however need different scopes for structs, enums, etc.
    class LocalScope : public Scope {
    public:
        // TODO: Implement Local Scope, etc.
    };

}// namespace Ceres::Binding

#endif//COMPILER_SCOPE_H
