#include "SourceManager.h"
#include <llvm/Support/WithColor.h>
#include <llvm/Support/raw_ostream.h>

namespace Ceres {

std::unique_ptr<SourceManager> SourceManager::singletonInstance = nullptr;

SourceManager& SourceManager::get()
{
    if (singletonInstance != nullptr) {
        return *singletonInstance;
    }
    singletonInstance.reset(new SourceManager);
    return *singletonInstance;
}

unsigned SourceManager::addSourceFileOrExit(std::string const& fileName)
{
    llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> fileOrError = llvm::MemoryBuffer::getFile(fileName);
    std::error_code error = fileOrError.getError();
    if (error) {
        llvm::WithColor::error(llvm::errs()) << "couldn't open " << fileName << ": " << error.message() << "\n";
        exit(1);
    }

    return sourceMgr.AddNewSourceBuffer(std::move(fileOrError.get()), llvm::SMLoc {});
}
llvm::MemoryBuffer const* SourceManager::getMemoryBuffer(unsigned int fileId)
{
    return sourceMgr.getMemoryBuffer(fileId);
}
llvm::SourceMgr& SourceManager::getLLVMSourceMgr() { return sourceMgr; }

} // namespace Ceres