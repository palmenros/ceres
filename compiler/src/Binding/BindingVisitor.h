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

#ifndef COMPILER_BINDINGVISITOR_H
#define COMPILER_BINDINGVISITOR_H

#include "../AST/ASTVisitor.h"

namespace Ceres::Binding {

    using namespace AST;

    class BindingVisitor : public ASTVisitor {
        // TODO: Override the needed visitor functions to perform the binding on the AST
    };

}// namespace Ceres::Binding

#endif//COMPILER_BINDINGVISITOR_H