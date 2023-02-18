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

#include "SymbolDeclaration.h"
#include "../AST/nodes/FunctionDefinition.h"

namespace Ceres::Binding {
    AST::FunctionParameter *getParam(SymbolDeclaration &symbol) {
        ASSERT(symbol.paramIdx.has_value());

        auto node = dynamic_cast<AST::FunctionDefinition *>(symbol.declarationNode);
        ASSERT(node != nullptr);
        ASSERT(node->parameters.size() < symbol.paramIdx.value());

        auto param = &node->parameters[symbol.paramIdx.value()];

        return param;
    }

    AST::Expression *getExpr(SymbolDeclaration &symbol) {
        auto node = dynamic_cast<AST::Expression *>(symbol.declarationNode);
        ASSERT(node != nullptr);

        return node;
    }

    // Constructors
    SymbolDeclaration::SymbolDeclaration(SymbolDeclarationKind kind, AST::Node *declarationNode)
        : kind(kind), declarationNode(declarationNode){};

    SymbolDeclaration::SymbolDeclaration(AST::Node *declarationNode, size_t param_idx)
        : kind(SymbolDeclarationKind::FunctionParamDeclaration), declarationNode(declarationNode),
          paramIdx(param_idx){};


    // Methods
    SymbolDeclarationKind SymbolDeclaration::getKind() { return kind; }

    AST::Node *SymbolDeclaration::getDeclarationNode() { return declarationNode; }

    std::optional<size_t> SymbolDeclaration::getParam_idx() { return paramIdx; }

    Type *SymbolDeclaration::getType() {
        if (kind == SymbolDeclarationKind::FunctionParamDeclaration) {
            return getParam(*this)->type;
        } else {
            return getExpr(*this)->type;
        }
    }
}// namespace Ceres::Binding
