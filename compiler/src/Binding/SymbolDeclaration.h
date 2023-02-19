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
#include <cstddef>
#include <optional>
#include <string>

namespace Ceres::Binding {

enum class SymbolDeclarationKind {
    FunctionDeclaration,
    LocalVariableDeclaration,
    GlobalVariableDeclaration,
    FunctionParamDeclaration,
};

class SymbolDeclaration {
private:
    SymbolDeclarationKind kind;
    AST::Node *declarationNode;
    std::optional<size_t> paramIdx = 0;

public:
    // TODO: sadly this is not rust so we cant create a constructor for a
    // variant, just pray no one uses this incorrectly
    SymbolDeclaration(SymbolDeclarationKind kind, AST::Node *declarationNode);
    SymbolDeclaration(size_t param_idx, AST::Node *declarationNode);

    // Methods
    SymbolDeclarationKind getKind();
    AST::Node *getDeclarationNode();
    std::optional<size_t> getParamIdx();
    Type *getType();
};

} // namespace Ceres::Binding

#endif // COMPILER_SYMBOLDECLARATION_H
