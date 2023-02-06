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

#ifndef COMPILER_NODE_H
#define COMPILER_NODE_H

#include "../SourceSpan.h"
#include <vector>

namespace Ceres::AST {

    class AbstractASTVisitor;

    class Node {
    public:
        SourceSpan sourceSpan;

    public:
        explicit Node(const SourceSpan &sourceSpan);

        virtual void accept(AbstractASTVisitor &visitor) = 0;
        virtual std::vector<Node *> getChildren() const = 0;

        virtual ~Node() = default;
    };

}// namespace Ceres::AST
#endif//COMPILER_NODE_H
