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
#include <unordered_map>

namespace Ceres::Binding {

    // Abstract class representing a scope
    class Scope {
        std::unordered_map<std::string, SymbolDeclaration> map;
        std::string name;
        Scope *enclosingScope;

    public:
        Scope(std::string name, Scope *enclosingScope)
            : name(name), enclosingScope(enclosingScope){};

        std::string getScopeName() { return name; };
        Scope *getEnclosingScope() { return enclosingScope; };

        void define(std::string &name, const SymbolDeclaration &symbol) {
            if (auto search = map.find(name); search != map.end()) {
                // TODO: dont panic
                Log::panic("duplicate symbol");
            } else {
                map[name] = symbol;
            }
        };

        SymbolDeclaration resolve(const std::string &name) {
            if (auto search = map.find(name); search != map.end()) {
                return map[name];
            } else {
                // TODO: dont panic
                Log::panic("undefined symbol");
            }
        };
    };
}// namespace Ceres::Binding

#endif//COMPILER_SCOPE_H
