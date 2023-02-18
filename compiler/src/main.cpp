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
#include "Typing/TypeVisitor.h"
#include "antlr4-runtime.h"

#include "llvm/ADT/APFloat.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/InitLLVM.h"

#include "AST/ASTStringifierVisitor.h"
#include "AST/ASTVisitor.h"
#include "AST/AntlrASTGeneratorVisitor.h"
#include "AST/nodes/CompilationUnit.h"
#include "Binding/BindingVisitor.h"
#include "Diagnostics/Diagnostics.h"
#include "Diagnostics/ParserErrorListener.h"
#include "Typing/TypeCheckVisitor.h"
#include "utils/InitCeres.h"
#include "utils/SourceManager.h"
#include "utils/log.hpp"

using namespace antlrgenerated;
using namespace antlr4;
using namespace Ceres;


int main(int argc, const char *argv[]) {
    // Performs initialization and destruction on scope end
    Ceres::InitCeres ceres;
    llvm::InitLLVM X(argc, argv);

    if (argc < 2) {
        std::cout << "Not enough arguments" << std::endl;
        std::cout << "Usage: '" << argv[0] << "' [filename]" << std::endl;
        return 1;
    }

    auto &sourceManager = SourceManager::get();
    unsigned fileId = sourceManager.addSourceFileOrExit(argv[1]);

    try {
        auto memoryBuffer = sourceManager.getMemoryBuffer(fileId);
        ANTLRInputStream input(memoryBuffer->getBufferStart(), memoryBuffer->getBufferSize());
        CeresLexer lexer(&input);
        CommonTokenStream tokens(&lexer);

        CeresParser parser(&tokens);
        parser.removeErrorListeners();

        std::unique_ptr<ParserErrorListener> parserErrorListener =
                std::make_unique<ParserErrorListener>(fileId);
        parser.addErrorListener(parserErrorListener.get());

        tree::ParseTree *tree = parser.compilationUnit();

        auto s = tree->toStringTree(&parser);
        //        Log::debug("Parse tree: {}", s);

        AST::AntlrASTGeneratorVisitor visitor{fileId};

        auto res = std::any_cast<AST::CompilationUnit *>(tree->accept(&visitor));
        ASSERT(res != nullptr);

        auto AST = std::unique_ptr<AST::CompilationUnit>(res);

        // AST::ASTStringifierVisitor stringifierVisitor;
        // auto str = stringifierVisitor.visit(*AST);
        // Log::info("AST: {}", str);

        Binding::BindingVisitor bindingVisitor;
        bindingVisitor.visit(*AST);
        Log::info("Binding visitor run!");

        Typing::TypeCheckVisitor typeCheckVisitor;
        typeCheckVisitor.visit(*AST);
        Log::info("Type check visitor run!");

        //        class Test : public Ceres::AST::ASTVisitor {
        //            void visitFunctionDefinition(AST::FunctionDefinition &def) override {
        //                Ceres::Diagnostics::report(def.functionNameSpan, Diag::err_function_identifier, def.functionName);
        //
        //                for (auto &param: def.parameters) {
        //                    Ceres::Diagnostics::report(param.parameterNameSourceSpan, Diag::err_param, {param.typeSourceSpan}, param.name);
        //                }
        //
        //                Ceres::AST::ASTVisitor::visitFunctionDefinition(def);
        //            }
        //        };
        //
        //        Test test;
        //        test.visit(*AST);

        // llvm::LLVMContext llvmContext;
        // auto module = std::make_unique<llvm::Module>("Hello", llvmContext);
        //    std::cout << "LLVM says: " << module->getName().str() << std::endl;
    } catch (std::exception &e) { Log::critical("Uncaught Exception: {}", e.what()); }
    return 0;
}
