/*
 * Copyright (C) 2023 Pedro Palacios Almendros
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

        SourceManager(SourceManager &other) = delete;
        void operator=(const SourceManager &) = delete;
        static SourceManager &get();

        /// Add a new Source File to the source Manager or fail with an error
        /// \param fileName fileName to open
        /// \return unsigned integer representing the file inside SourceManager
        unsigned addSourceFileOrExit(const std::string &fileName);

        const llvm::MemoryBuffer *getMemoryBuffer(unsigned fileId);
        llvm::SourceMgr &getLLVMSourceMgr();
    };
}// namespace Ceres
#endif//COMPILER_SOURCEMANAGER_H
