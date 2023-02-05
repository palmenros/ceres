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

#include "FunctionDefinition.h"

#include <utility>

namespace Ceres::AST {
    FunctionDefinition::FunctionDefinition(const SourceSpan &sourceSpan, FunctionVisibility visibility,
                                           std::string functionName,
                                           std::vector<FunctionParameter> parameters, Type returnType,
                                           std::unique_ptr<BlockStatement> &&block,
                                           const SourceSpan &returnTypeSpan, const SourceSpan &functionNameSpan) : Node(
            sourceSpan), visibility(visibility), functionName(std::move(functionName)), parameters(std::move(parameters)), returnType(std::move(
            returnType)), block(std::move(block)), returnTypeSpan(returnTypeSpan), functionNameSpan(functionNameSpan) {}
} // AST