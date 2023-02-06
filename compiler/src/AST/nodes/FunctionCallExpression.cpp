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

#include "FunctionCallExpression.h"

#include "../AbstractASTVisitor.h"
#include <utility>

namespace Ceres::AST {
    FunctionCallExpression::FunctionCallExpression(const SourceSpan &sourceSpan, std::string functionIdentifier,
                                                   std::vector<std::unique_ptr<Expression>> &&arguments,
                                                   SourceSpan functionIdentifierSourceSpan)
        : Expression(sourceSpan), functionIdentifier(std::move(functionIdentifier)), arguments(std::move(arguments)),
          functionIdentifierSourceSpan(functionIdentifierSourceSpan) {}

    void FunctionCallExpression::accept(AbstractASTVisitor &visitor) {
        visitor.visitFunctionCallExpression(*this);
    }

    std::vector<Node *> FunctionCallExpression::getChildren() const {
        // TODO: In the future, when we add support for function calls to pointers, we need to add the expression
        //          resolving to the function pointer to the children
        std::vector<Node *> v;
        v.reserve(arguments.size());
        for (auto &a: arguments) {
            v.push_back(a.get());
        }
        return v;
    }
}// namespace Ceres::AST