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

#ifndef COMPILER_LOG_HPP
#define COMPILER_LOG_HPP

#include "spdlog/spdlog.h"

namespace Ceres::Log {

    // Set up spdlog for logging
    void setupLogging() {
        spdlog::set_pattern("%^[%l]%$ %v");
    }

    // Just wrappers to call spdlog

    template <typename ...Args>
    void debug(Args && ...args) {
        spdlog::debug(std::forward<Args>(args)...);
    }

    template <typename ...Args>
    void info(Args && ...args) {
        spdlog::info(std::forward<Args>(args)...);
    }

    template <typename ...Args>
    void warn(Args && ...args) {
        spdlog::warn(std::forward<Args>(args)...);
    }

    template <typename ...Args>
    void error(Args && ...args) {
        spdlog::error(std::forward<Args>(args)...);
    }

    template <typename ...Args>
    void critical(Args && ...args) {
        spdlog::critical(std::forward<Args>(args)...);
    }

    // Panic support

    #if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
        #define CERES_PRETTY_FUNCTION_NAME __FUNCSIG__
    #else
        #define CERES_PRETTY_FUNCTION_NAME __PRETTY_FUNCTION__
    #endif

    template <typename ...Args>
    void panic(Args && ...args) {
        critical(std::forward<Args>(args)...);
        exit(1);
    }

    // Macro for NOT_IMPLEMENTED
    #define NOT_IMPLEMENTED() ::Ceres::Log::panic("Not implemented: {}", CERES_PRETTY_FUNCTION_NAME)

}

#endif //COMPILER_LOG_HPP
