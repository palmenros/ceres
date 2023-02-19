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

#include "ParserErrorListener.h"
#include "../utils/log.hpp"
#include "Diagnostics.h"

namespace Ceres {
void ParserErrorListener::syntaxError(antlr4::Recognizer *recognizer,
                                      antlr4::Token *offendingSymbol,
                                      size_t line, size_t charPositionInLine,
                                      const std::string &msg,
                                      std::exception_ptr e) {
    auto loc = SourceManager::get().getLLVMSourceMgr().FindLocForLineAndColumn(
        fileId, line, (unsigned int)charPositionInLine);
    Diagnostics::report(loc, Diag::parse_error, msg);
}

void ParserErrorListener::reportAmbiguity(antlr4::Parser *recognizer,
                                          const antlr4::dfa::DFA &dfa,
                                          size_t startIndex, size_t stopIndex,
                                          bool exact,
                                          const antlrcpp::BitSet &ambigAlts,
                                          antlr4::atn::ATNConfigSet *configs) {
    Log::panic("Parser detected an ambiguity in the grammar! StartIndex: {}, "
               "StopIndex: {}",
               startIndex, stopIndex);
}

ParserErrorListener::ParserErrorListener(unsigned int fileId)
    : fileId(fileId) {}
} // namespace Ceres