
#ifndef COMPILER_CODEGENERATOR_H
#define COMPILER_CODEGENERATOR_H

#include "CodegenVisitor.h"
namespace Ceres::Codegen {

class CodeGenerator {
    CodegenVisitor visitor;

public:
    CodeGenerator(llvm::LLVMContext* context);

    void generateCode(AST::CompilationUnit& compilationUnit);
};

} // Codegen

#endif // COMPILER_CODEGENERATOR_H
