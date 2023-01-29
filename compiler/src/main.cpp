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

#include "antlr4-runtime.h"
#include "TLexer.h"
#include "TParser.h"

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

int main(int argc, const char * argv[]) {

//    ANTLRInputStream input("a = b + \"c\";(((x * d))) * e + f; a + (x * (y ? 0 : 1) + z);");
    ANTLRInputStream input("a = pedro; c = h + d * b;");
    TLexer lexer(&input);
    CommonTokenStream tokens(&lexer);

    TParser parser(&tokens);
    tree::ParseTree *tree = parser.main();

    auto s = tree->toStringTree(&parser);
    std::cout << "Parse Tree: " << s << std::endl;

    llvm::LLVMContext llvmContext;
    auto module = std::make_unique<llvm::Module>("Hello", llvmContext);
    std::cout << "LLVM says: " << module->getName().str() << std::endl;


    return 0;
}