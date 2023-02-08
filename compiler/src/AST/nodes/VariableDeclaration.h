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

#ifndef COMPILER_VARIABLEDECLARATION_H
#define COMPILER_VARIABLEDECLARATION_H

#include "../../Binding/Type.h"
#include "Expression.h"
#include "Statement.h"
#include <memory>
#include <string>

namespace Ceres::AST {

    enum class VariableVisibility {
        Public,
        Private
    };

    enum class VariableScope {
        Local,// Defined inside a function
        Global// Defined as a global variable, outside any function
    };

    enum class VariableConstness {
        Const,
        NonConst
    };

    class VariableDeclaration : public Statement {
    public:
        VariableVisibility visibility;
        VariableConstness constness;
        VariableScope scope;

        Type type;
        std::string identifier;

        SourceSpan typeSourceSpan;
        SourceSpan identifierSourceSpan;

        // Can be nullptr if the variable doesn't have initializer expression
        std::unique_ptr<Expression> initializerExpression;

    public:
        VariableDeclaration(const Ceres::SourceSpan &sourceSpan,
                            std::unique_ptr<Expression> &&initializerExpression,
                            VariableVisibility visibility, VariableConstness constness, VariableScope scope,
                            Type type, std::string identifier, const SourceSpan &typeSourceSpan,
                            const SourceSpan &identifierSourceSpan);

        void accept(AbstractASTVisitor &visitor) override;

        std::vector<Node *> getChildren() const override;
    };

}// namespace Ceres::AST

#endif//COMPILER_VARIABLEDECLARATION_H
