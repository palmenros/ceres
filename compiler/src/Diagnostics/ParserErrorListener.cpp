#include "ParserErrorListener.h"
#include "../utils/log.hpp"
#include "Diagnostics.h"

namespace Ceres {
void ParserErrorListener::syntaxError(antlr4::Recognizer* recognizer, antlr4::Token* offendingSymbol, size_t line,
    size_t charPositionInLine, std::string const& msg, std::exception_ptr e)
{
    auto loc = SourceManager::get().getLLVMSourceMgr().FindLocForLineAndColumn(
        fileId, line, (unsigned int)charPositionInLine);
    Diagnostics::report(loc, Diag::parse_error, msg);
}

void ParserErrorListener::reportAmbiguity(antlr4::Parser* recognizer, antlr4::dfa::DFA const& dfa, size_t startIndex,
    size_t stopIndex, bool exact, antlrcpp::BitSet const& ambigAlts, antlr4::atn::ATNConfigSet* configs)
{
    Log::panic("Parser detected an ambiguity in the grammar! StartIndex: {}, "
               "StopIndex: {}",
        startIndex, stopIndex);
}

ParserErrorListener::ParserErrorListener(unsigned int fileId)
    : fileId(fileId)
{
}
} // namespace Ceres