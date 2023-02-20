#ifndef COMPILER_SOURCEMANAGER_H
#define COMPILER_SOURCEMANAGER_H

#include "llvm/Support/SourceMgr.h"

namespace Ceres {
/* Singleton class */
class SourceManager {
protected:
    SourceManager() = default;

    static std::unique_ptr<SourceManager> singletonInstance;

public:
    llvm::SourceMgr sourceMgr;

    SourceManager(SourceManager& other) = delete;
    void operator=(SourceManager const&) = delete;
    static SourceManager& get();

    /// Add a new Source File to the source Manager or fail with an error
    /// \param fileName fileName to open
    /// \return unsigned integer representing the file inside SourceManager
    unsigned addSourceFileOrExit(std::string const& fileName);

    llvm::MemoryBuffer const* getMemoryBuffer(unsigned fileId);
    llvm::SourceMgr& getLLVMSourceMgr();
};
} // namespace Ceres
#endif // COMPILER_SOURCEMANAGER_H
