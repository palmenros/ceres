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

#ifndef COMPILER_SOURCESPAN_H
#define COMPILER_SOURCESPAN_H

#include <cstddef>

namespace Ceres::AST {



    struct SourceSpan {

    private:

        explicit SourceSpan(bool isSpanValid);

    public:

        // StartLine and EndLine start at 1
        size_t startLine, endLine;

        // CAUTION: The character indices start at 0, while many editors start at 1.
        // TODO: Maybe change these to be 1-indexed?
        size_t startCharacterIndex, endCharacterIndex;

        // Does the span contain valid data? Maybe it doesn't
        bool isSpanValid;

        SourceSpan(size_t startLine, size_t endLine, size_t startCharacterIndex,
                   size_t endCharacterIndex);

        static SourceSpan createInvalidSpan();
    };
}


#endif //COMPILER_SOURCESPAN_H
