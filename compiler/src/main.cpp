#include <iostream>
#include <string>

#include "CeresLexer.h"
#include "CeresParser.h"
#include "Flow/FlowCheckVisitor.h"
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
#include "Codegen/CodeGenerator.h"
#include "Diagnostics/Diagnostics.h"
#include "Diagnostics/ParserErrorListener.h"
#include "Typing/TypeCheckVisitor.h"
#include "utils/InitCeres.h"
#include "utils/SourceManager.h"
#include "utils/log.hpp"

using namespace antlrgenerated;
using namespace antlr4;
using namespace Ceres;

int main(int argc, char const* argv[])
{
    // Performs initialization and destruction on scope end
    Ceres::InitCeres ceres;
    llvm::InitLLVM X(argc, argv);

    if (argc < 2) {
        std::cout << "Not enough arguments" << std::endl;
        std::cout << "Usage: '" << argv[0] << "' [filename]" << std::endl;
        return 1;
    }

    auto& sourceManager = SourceManager::get();
    unsigned fileId = sourceManager.addSourceFileOrExit(argv[1]);

    try {
        auto const* memoryBuffer = sourceManager.getMemoryBuffer(fileId);
        ANTLRInputStream input(memoryBuffer->getBufferStart(), memoryBuffer->getBufferSize());
        CeresLexer lexer(&input);
        CommonTokenStream tokens(&lexer);

        CeresParser parser(&tokens);
        parser.removeErrorListeners();

        std::unique_ptr<ParserErrorListener> parserErrorListener = std::make_unique<ParserErrorListener>(fileId);
        parser.addErrorListener(parserErrorListener.get());

        tree::ParseTree* tree = parser.compilationUnit();

        auto s = tree->toStringTree(&parser);
        //        Log::debug("Parse tree: {}", s);

        AST::AntlrASTGeneratorVisitor visitor { fileId };

        auto* res = std::any_cast<AST::CompilationUnit*>(tree->accept(&visitor));
        ASSERT(res != nullptr);

        auto AST = std::unique_ptr<AST::CompilationUnit>(res);

        Binding::BindingVisitor bindingVisitor;
        bindingVisitor.visit(*AST);
        //        Log::info("Binding visitor run!");

        Typing::TypeCheckVisitor typeCheckVisitor;
        typeCheckVisitor.visit(*AST);
        //        Log::info("Type check visitor run!");

        Typing::FlowCheckVisitor flowCheckVisitor;
        flowCheckVisitor.visit(*AST);
        //        Log::info("Flow check visitor run!");

        // If there's any errors, bail out
        if (Diagnostics::getNumErrors() != 0) {
            return 1;
        }

        Codegen::CodeGenerator codeGenerator(InitCeres::getLLVMContext());
        codeGenerator.generateCode(*AST);
        //        Log::info("Code generation run!");

        // AST::ASTStringifierVisitor stringifierVisitor;
        // auto str = stringifierVisitor.visit(*AST);
        // Log::info("AST: {}", str);

        //        class Test : public Ceres::AST::ASTVisitor {
        //            void visitFunctionDefinition(AST::FunctionDefinition &def)
        //            override {
        //                Ceres::Diagnostics::report(def.functionNameSpan,
        //                Diag::err_function_identifier, def.functionName);
        //
        //                for (auto &param: def.parameters) {
        //                    Ceres::Diagnostics::report(param.parameterNameSourceSpan,
        //                    Diag::err_param, {param.typeSourceSpan},
        //                    param.name);
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
        //    std::cout << "LLVM says: " << module->getName().str() <<
        //    std::endl;
    } catch (std::exception& e) {
        Log::critical("Uncaught Exception: {}", e.what());
    }
    return 0;
}
