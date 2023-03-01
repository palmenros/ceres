#ifndef COMPILER_INITCERES_H
#define COMPILER_INITCERES_H

#include <llvm/IR/LLVMContext.h>
namespace Ceres {

// The purpose of this class is to perform initialization and destruction
// actions for the Ceres compiler
class InitCeres {
private:
    std::unique_ptr<llvm::LLVMContext> LLVMContext;
    static InitCeres* instance;

public:
    InitCeres();
    ~InitCeres();

    static llvm::LLVMContext* getLLVMContext();

    // Delete assignment operator
    InitCeres& operator=(InitCeres const&) = delete;
    InitCeres(InitCeres const&) = delete;

    // Delete move assignment operator
    InitCeres(InitCeres&&) = delete;
    InitCeres& operator=(InitCeres&&) = delete;
};

} // namespace Ceres

#endif // COMPILER_INITCERES_H
