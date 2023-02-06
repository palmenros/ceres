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

#include "CompilationUnit.h"
#include "../AbstractASTVisitor.h"

namespace Ceres::AST {
    CompilationUnit::CompilationUnit(const SourceSpan &sourceSpan,
                                     std::vector<std::unique_ptr<FunctionDefinition>> &&functionDefinitions,
                                     std::vector<std::unique_ptr<VariableDeclaration>> &&globalVariableDeclarations)
        : Node(sourceSpan),
          functionDefinitions(std::move(functionDefinitions)),
          globalVariableDeclarations(std::move(globalVariableDeclarations)) {}

    void CompilationUnit::accept(AbstractASTVisitor &visitor) {
        visitor.visitCompilationUnit(*this);
    }

    std::vector<Node *> CompilationUnit::getChildren() const {
        std::vector<Node *> v;
        v.reserve(functionDefinitions.size() + globalVariableDeclarations.size());

        for (auto &ptr: functionDefinitions) {
            v.push_back(ptr.get());
        }

        for (auto &ptr: globalVariableDeclarations) {
            v.push_back(ptr.get());
        }
        return v;
    }

    CompilationUnit::CompilationUnit(const SourceSpan &sourceSpan) : Node(sourceSpan) {}
}// namespace Ceres::AST