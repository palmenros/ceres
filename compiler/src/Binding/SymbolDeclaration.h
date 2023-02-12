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

#ifndef COMPILER_SYMBOLDECLARATION_H
#define COMPILER_SYMBOLDECLARATION_H

#include "../AST/nodes/Node.h"
#include "../Typing/Type.h"
#include <string>

namespace Ceres::Binding {

    enum class SymbolDeclarationKind {
        FunctionDeclaration,
        LocalVariableDeclaration,
        GlobalVariableDeclaration
    };

    class SymbolDeclaration {
    public:
        SymbolDeclarationKind kind;
        AST::Node *declarationNode;

        SymbolDeclaration(SymbolDeclarationKind kind, AST::Node *declarationNode)
            : kind(kind), declarationNode(declarationNode){};
    };

}// namespace Ceres::Binding

#endif//COMPILER_SYMBOLDECLARATION_H
