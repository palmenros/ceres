/*
 * Copyright (C) 2023 Pedro Palacios Almendros, 2023 Daniel Martin Gomez
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

#include <iostream>
#include <string>

#include "CeresLexer.h"
#include "CeresParser.h"
#include "antlr4-runtime.h"

#include "llvm/ADT/APFloat.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"

#include "AST/ASTStringifierVisitor.h"
#include "AST/ASTVisitor.h"
#include "AST/AntlrASTGeneratorVisitor.h"
#include "AST/nodes/CompilationUnit.h"
#include "utils/log.hpp"

using namespace antlrgenerated;
using namespace antlr4;
using namespace Ceres;


int main(int argc, const char *argv[]) {
    Log::setupLogging();

    if (argc < 2) {
        std::cout << "Not enough arguments" << std::endl;
        std::cout << "Usage: '" << argv[0] << "' [filename]" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        Log::error("Could not open file '{}'", argv[1]);
        return 1;
    }

    try {

        ANTLRInputStream input(file);
        CeresLexer lexer(&input);
        CommonTokenStream tokens(&lexer);

        CeresParser parser(&tokens);
        tree::ParseTree *tree = parser.compilationUnit();

        auto s = tree->toStringTree(&parser);
        Log::debug("Parse tree: {}", s);

        AST::AntlrASTGeneratorVisitor visitor;

        auto res = std::any_cast<AST::CompilationUnit *>(tree->accept(&visitor));
        ASSERT(res != nullptr);

        auto AST = std::unique_ptr<AST::CompilationUnit>(res);

        AST::ASTStringifierVisitor stringifierVisitor;
        auto str = stringifierVisitor.visit(*AST);
        Log::info("AST: {}", str);

        llvm::LLVMContext llvmContext;
        auto module = std::make_unique<llvm::Module>("Hello", llvmContext);
        //    std::cout << "LLVM says: " << module->getName().str() << std::endl;
    } catch (std::exception &e) {
        Log::critical("Uncaught Exception: {}", e.what());
    }
    return 0;
}
