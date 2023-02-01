#include <iostream>
#include <string>

#include "antlr4-runtime.h"
#include "CeresLexer.h"
#include "CeresParser.h"

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
    CeresLexer lexer(&input);
    CommonTokenStream tokens(&lexer);

    CeresParser parser(&tokens);
    tree::ParseTree *tree = parser.start();

    auto s = tree->toStringTree(&parser);
    std::cout << s << std::endl;

    llvm::LLVMContext llvmContext;
    auto module = std::make_unique<llvm::Module>("Hello", llvmContext);
//    std::cout << "LLVM says: " << module->getName().str() << std::endl;

    return 0;
}
