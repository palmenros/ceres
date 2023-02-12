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

#ifndef COMPILER_TYPEVISITOR_H
#define COMPILER_TYPEVISITOR_H

#include "Type.h"

namespace Ceres::Typing {

    class TypeVisitor {
    public:
        void visit(Type *type);

        // Note: This visitor uses pointers instead of references due to the
        //  unique nature of types. A type is uniquely represented by its
        //  pointer value, due to caching.

        virtual void visitUnitVoidType(UnitVoidType *type) = 0;
        virtual void visitUnresolvedType(UnresolvedType *type) = 0;
        virtual void visitNotYetInferredType(NotYetInferredType *type) = 0;
        virtual void visitPrimitiveScalarType(PrimitiveScalarType *type) = 0;
        virtual void visitFunctionType(FunctionType *type) = 0;
    };

}// namespace Ceres::Typing

#endif//COMPILER_TYPEVISITOR_H
