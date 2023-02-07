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

#include "SourceManager.h"
#include <llvm/Support/WithColor.h>
#include <llvm/Support/raw_ostream.h>

namespace Ceres {

    SourceManager *SourceManager::singletonInstance = nullptr;

    SourceManager &SourceManager::get() {
        if (singletonInstance != nullptr) {
            return *singletonInstance;
        }
        singletonInstance = new SourceManager();
        return *singletonInstance;
    }

    unsigned SourceManager::addSourceFileOrExit(const std::string &fileName) {
        llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> fileOrError = llvm::MemoryBuffer::getFile(fileName);
        std::error_code error = fileOrError.getError();
        if (error) {
            llvm::WithColor::error(llvm::errs()) << "couldn't open " << fileName << ": " << error.message() << "\n";
            exit(1);
        }

        return sourceMgr.AddNewSourceBuffer(std::move(fileOrError.get()), llvm::SMLoc{});
    }
    const llvm::MemoryBuffer *SourceManager::getMemoryBuffer(unsigned int fileId) {
        return sourceMgr.getMemoryBuffer(fileId);
    }
    llvm::SourceMgr &SourceManager::getLLVMSourceMgr() {
        return sourceMgr;
    }

}// namespace Ceres