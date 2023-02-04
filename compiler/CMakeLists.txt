# Copyright (C) 2023 Pedro Palacios Almendros, 2023 Daniel Martin Gomez
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

CMAKE_MINIMUM_REQUIRED(VERSION 3.7 FATAL_ERROR)

# Project name
project(compiler)

# Add ./cmake folder as module path. Used to load ANTLR4 external module
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Use C++17
set(CMAKE_CXX_STANDARD 17)

# Define Compiler executable and add our sources
add_executable(compiler src/main.cpp src/utils/log.hpp src/AST/AntlrASTGeneratorVisitor.cpp src/AST/AntlrASTGeneratorVisitor.h src/AST/Node.cpp src/AST/Node.h src/AST/SourceSpan.cpp src/AST/SourceSpan.h src/AST/CompilationUnit.cpp src/AST/CompilationUnit.h src/AST/FunctionDefinition.cpp src/AST/FunctionDefinition.h src/AST/VariableDeclaration.cpp src/AST/VariableDeclaration.h src/AST/Expression.cpp src/AST/Expression.h src/AST/Type.cpp src/AST/Type.h src/AST/CommaExpression.cpp src/AST/CommaExpression.h src/AST/IntLiteralExpression.cpp src/AST/IntLiteralExpression.h src/AST/FloatLiteralExpression.cpp src/AST/FloatLiteralExpression.h src/AST/BoolLiteral.cpp src/AST/BoolLiteral.h src/AST/IdentifierExpression.cpp src/AST/IdentifierExpression.h src/AST/BinaryOperationExpression.cpp src/AST/BinaryOperationExpression.h)

##########################################
#           START LLVM
##########################################

include_directories(${PROJECT_SOURCE_DIR}/thirdparty/spdlog/include)

##########################################
#           END LLVM
##########################################

##########################################
#           START LLVM
##########################################

find_package(LLVM 15 REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

target_include_directories(compiler SYSTEM PUBLIC ${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

# Find the libraries that correspond to the LLVM components that we wish to use

# A list of components can be found by executing "llvm-config --components"
llvm_map_components_to_libnames(llvm_libs support core irreader)

# Link against LLVM libraries
target_link_libraries(compiler ${llvm_libs})

##########################################
#           END LLVM
##########################################

##########################################
#           START ANTLR 4
##########################################

# required if linking to static library
add_definitions(-DANTLR4CPP_STATIC)

if (MSVC)
    # using /MD flag for antlr4_runtime (for Visual C++ compilers only)
    set(ANTLR4_WITH_STATIC_CRT OFF)
    message("Detected MSVC. Setting ANTLR4_WITH_STATIC_CRT OFF. If you are not using MSVC, comment this line out.")
endif (MSVC)

# Specify the version of the antlr4 library needed for this project.
# By default the latest version of antlr4 will be used.  You can specify a
# specific, stable version by setting a repository tag value or a link
# to a zip file containing the libary source.
set(ANTLR4_TAG 4.11.1)
#set(ANTLR4_ZIP_REPOSITORY https://github.com/antlr/antlr4/archive/refs/tags/4.11.1.zip)

# Add external build for antlrcpp
include(ExternalAntlr4Cpp)

# Add ANTLR4cpp artifacts to project environment
include_directories(${ANTLR4_INCLUDE_DIRS})

# set variable pointing to the antlr tool that supports C++
# this is not required if the jar file can be found under PATH environment
set(ANTLR_EXECUTABLE ${PROJECT_SOURCE_DIR}/thirdparty/antlr/antlr-4.11.1-complete.jar)

# Add macros to generate ANTLR Cpp code from grammar
find_package(ANTLR REQUIRED)

# Macros for building Lexer and parser

antlr_target(Lexer                                      # ANTLR Target Name. Can be changed.
        ${PROJECT_SOURCE_DIR}/syntax/CeresLexer.g4          # Lexer .g4 syntax file
        LEXER                                           # Type: LEXER, PARSER, LISTENER, VISITOR
        PACKAGE antlrgenerated                          # C++ namespace for generated code
        )

antlr_target(Parser                                     # ANTLR Target Name. Can be changed.
        ${PROJECT_SOURCE_DIR}/syntax/CeresParser.g4         # Lexer .g4 syntax file
        PARSER VISITOR                                  # Type: LEXER, PARSER, LISTENER, VISITOR
        PACKAGE antlrgenerated                          # C++ namespace for generated code
        DEPENDS_ANTLR Lexer                             # Needs .token files generated by Lexer
        COMPILE_FLAGS -lib ${ANTLR_Lexer_OUTPUT_DIR}    # Include path to Lexer generated output
        )

# include generated files in project environment
include_directories(${ANTLR_Lexer_OUTPUT_DIR})
include_directories(${ANTLR_Parser_OUTPUT_DIR})

# add generated grammar to demo binary target
target_sources(compiler PRIVATE
        ${ANTLR_Lexer_CXX_OUTPUTS}
        ${ANTLR_Parser_CXX_OUTPUTS})
target_link_libraries(compiler antlr4_static)

##########################################
#           END ANTLR 4
##########################################

