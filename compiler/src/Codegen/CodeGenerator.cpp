#include "CodeGenerator.h"
#include "../Diagnostics/Diagnostics.h"

#include <llvm/IR/LegacyPassManager.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>

namespace Ceres::Codegen {

CodeGenerator::CodeGenerator(llvm::LLVMContext* context)
    : visitor(context)
{
    ASSERT(Diagnostics::getNumErrors() == 0);
}

void CodeGenerator::generateCode(AST::CompilationUnit& compilationUnit)
{
    visitor.visit(compilationUnit);
    // TODO: Better printing of this
    visitor.module->print(llvm::errs(), nullptr);

    // TODO: Use parameters for output filename
    auto targetTriple = llvm::sys::getDefaultTargetTriple();

    // Initialize all targets for emitting code
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    std::string error;
    auto const* target = llvm::TargetRegistry::lookupTarget(targetTriple, error);

    if (!target) {
        Log::panic("Target lookup failed: {}", error);
    }

    // TODO: Pass all this options via command line parameters
    auto const* CPU = "generic";
    auto const* features = "";

    llvm::TargetOptions opt;
    auto relocationModel = llvm::Optional<llvm::Reloc::Model>();
    auto* targetMachine = target->createTargetMachine(targetTriple, CPU, features, opt, relocationModel);
    visitor.module->setDataLayout(targetMachine->createDataLayout());
    visitor.module->setTargetTriple(targetTriple);

    auto const* filename = "output.o";
    std::error_code error_code;
    llvm::raw_fd_ostream dest(filename, error_code, llvm::sys::fs::OF_None);

    if (error_code) {
        Log::panic("Could not open output file '{}': {}", filename, error_code.message());
    }

    llvm::legacy::PassManager pass;
    auto fileType = llvm::CGFT_ObjectFile;

    if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, fileType)) {
        Log::panic("TargetMachine can't emit a file of this type");
    }

    pass.run(*visitor.module);
    dest.flush();
}

} // Codegen