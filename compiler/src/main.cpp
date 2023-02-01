/* Copyright (c) 2012-2017 The ANTLR Project. All rights reserved.
 * Use of this file is governed by the BSD 3-clause license that
 * can be found in the LICENSE.txt file in the project root.
 */

//
//  main.cpp
//  antlr4-cpp-demo
//
//  Created by Mike Lischke on 13.03.16.
//

#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>

#include "antlr4-runtime.h"
#include "SLCLexer.h"
#include "SLCParser.h"

#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Verifier.h"

using namespace antlrgenerated;
using namespace antlr4;

int main(int argc, const char *argv[])
{
    if (argc < 2)
	return -1;

    std::ifstream file(argv[1], std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    if (!file.read(buffer.data(), size))
    	return -1;

    ANTLRInputStream input(buffer.data());
    SLCLexer lexer(&input);
    CommonTokenStream tokens(&lexer);

    SLCParser parser(&tokens);
    tree::ParseTree *tree = parser.start();

    auto s = tree->toStringTree(&parser);
    std::cout << s << std::endl;

    llvm::LLVMContext llvmContext;
    auto module = std::make_unique<llvm::Module>("Hello", llvmContext);
//    std::cout << "LLVM says: " << module->getName().str() << std::endl;

    return 0;
}
