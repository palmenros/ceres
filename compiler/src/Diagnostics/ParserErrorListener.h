#ifndef COMPILER_PARSERERRORLISTENER_H
#define COMPILER_PARSERERRORLISTENER_H

#include "BaseErrorListener.h"

namespace Ceres {

class ParserErrorListener : public antlr4::BaseErrorListener {
protected:
    unsigned fileId;

public:
    explicit ParserErrorListener(unsigned int fileId);

    void syntaxError(antlr4::Recognizer* recognizer, antlr4::Token* offendingSymbol, size_t line,
        size_t charPositionInLine, std::string const& msg, std::exception_ptr e) override;
    void reportAmbiguity(antlr4::Parser* recognizer, antlr4::dfa::DFA const& dfa, size_t startIndex, size_t stopIndex,
        bool exact, antlrcpp::BitSet const& ambigAlts, antlr4::atn::ATNConfigSet* configs) override;
};

} // namespace Ceres

#endif // COMPILER_PARSERERRORLISTENER_H
