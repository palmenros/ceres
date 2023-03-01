#include "CodeGenerator.h"
#include "../Diagnostics/Diagnostics.h"

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
}

} // Codegen