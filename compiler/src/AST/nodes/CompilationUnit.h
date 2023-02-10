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

#ifndef COMPILER_COMPILATIONUNIT_H
#define COMPILER_COMPILATIONUNIT_H

#include "FunctionDefinition.h"
#include "Node.h"
#include "VariableDeclaration.h"
#include <memory>
#include <vector>

namespace Ceres::AST {

    class CompilationUnit : public Node {
    public:
        std::vector<std::unique_ptr<FunctionDefinition>> functionDefinitions;
        std::vector<std::unique_ptr<VariableDeclaration>> globalVariableDeclarations;

        CompilationUnit(
                const SourceSpan &sourceSpan,
                std::vector<std::unique_ptr<FunctionDefinition>> &&functionDefinitions,
                std::vector<std::unique_ptr<VariableDeclaration>> &&globalVariableDeclarations);

        CompilationUnit(const SourceSpan &sourceSpan);

        void accept(AbstractASTVisitor &visitor) override;

        std::vector<Node *> getChildren() const override;
    };

}// namespace Ceres::AST

#endif//COMPILER_COMPILATIONUNIT_H
