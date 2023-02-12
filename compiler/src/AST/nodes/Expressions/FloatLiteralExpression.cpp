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

#include "FloatLiteralExpression.h"

#include "../../AbstractASTVisitor.h"
#include <utility>

namespace Ceres::AST {

    FloatLiteralExpression::FloatLiteralExpression(const SourceSpan &sourceSpan,
                                                   FloatLiteralBase base, Type *type,
                                                   std::string str)
        : Expression(sourceSpan, type), base(base), str(std::move(str)) {}

    void FloatLiteralExpression::accept(AbstractASTVisitor &visitor) {
        visitor.visitFloatLiteralExpression(*this);
    }

    std::vector<Node *> FloatLiteralExpression::getChildren() const { return {}; }
}// namespace Ceres::AST