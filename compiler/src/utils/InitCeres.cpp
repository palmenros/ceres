#include "InitCeres.h"
#include "log.hpp"
#include <iostream>

namespace Ceres {

InitCeres* InitCeres::instance = nullptr;

InitCeres::InitCeres()
{
    if (instance != nullptr) {
        std::cerr << "Fatal Compiler Error: Creating two InitCeres!" << std::endl;
    }

    instance = this;
    LLVMContext = std::make_unique<llvm::LLVMContext>();
    Log::setupLogging();
}

InitCeres::~InitCeres() { instance = nullptr; }

llvm::LLVMContext* InitCeres::getLLVMContext()
{
    ASSERT(instance != nullptr);
    return instance->LLVMContext.get();
}

} // namespace Ceres