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

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add ./cmake folder as module path. Used to load ANTLR4 external module
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Use C++17
set(CMAKE_CXX_STANDARD 17)

# Define Compiler executable and add our sources
add_executable(compiler src/Typing/Type.cpp src/Typing/Type.h
        src/main.cpp src/utils/log.hpp src/AST/AntlrASTGeneratorVisitor.cpp src/AST/AntlrASTGeneratorVisitor.h
        src/AST/nodes/Node.cpp src/AST/nodes/Node.h src/utils/SourceSpan.cpp src/utils/SourceSpan.h
        src/AST/nodes/CompilationUnit.cpp src/AST/nodes/CompilationUnit.h src/AST/nodes/FunctionDefinition.cpp
        src/AST/nodes/FunctionDefinition.h src/AST/nodes/VariableDeclaration.cpp src/AST/nodes/VariableDeclaration.h
        src/AST/nodes/Expression.cpp src/AST/nodes/Expression.h src/AST/nodes/CommaExpression.cpp
        src/AST/nodes/CommaExpression.h src/AST/nodes/IntLiteralExpression.cpp src/AST/nodes/IntLiteralExpression.h
        src/AST/nodes/FloatLiteralExpression.cpp src/AST/nodes/FloatLiteralExpression.h src/AST/nodes/BoolLiteral.cpp
        src/AST/nodes/BoolLiteral.h src/AST/nodes/IdentifierExpression.cpp src/AST/nodes/IdentifierExpression.h
        src/AST/nodes/BinaryOperationExpression.cpp src/AST/nodes/BinaryOperationExpression.h
        src/AST/nodes/PostfixExpression.cpp src/AST/nodes/PostfixExpression.h src/AST/nodes/PrefixExpression.cpp
        src/AST/nodes/PrefixExpression.h src/AST/nodes/FunctionCallExpression.cpp src/AST/nodes/FunctionCallExpression.h
        src/AST/nodes/AssignmentExpression.cpp src/AST/nodes/AssignmentExpression.h src/AST/FunctionParameter.cpp
        src/AST/FunctionParameter.h src/AST/nodes/Statement.cpp src/AST/nodes/Statement.h src/AST/nodes/BlockStatement.cpp
        src/AST/nodes/BlockStatement.h src/AST/nodes/VariableDeclarationStatement.cpp
        src/AST/nodes/VariableDeclarationStatement.h src/AST/nodes/ExpressionStatement.cpp
        src/AST/nodes/ExpressionStatement.h src/AST/nodes/ReturnStatement.cpp src/AST/nodes/ReturnStatement.h
        src/AST/nodes/IfStatement.cpp src/AST/nodes/IfStatement.h src/AST/nodes/WhileStatement.cpp
        src/AST/nodes/WhileStatement.h src/AST/nodes/ForStatement.cpp src/AST/nodes/ForStatement.h
        src/AST/AbstractASTVisitor.cpp src/AST/AbstractASTVisitor.h src/AST/ASTVisitor.cpp src/AST/ASTVisitor.h
        src/AST/AbstractReturningASTVisitor.hpp src/AST/ReturningASTVisitor.hpp src/AST/ASTStringifierVisitor.cpp
        src/AST/ASTStringifierVisitor.h src/utils/SourceManager.cpp src/utils/SourceManager.h
        src/Diagnostics/Diagnostics.def src/Diagnostics/Diagnostics.cpp src/Diagnostics/Diagnostics.h
        src/Diagnostics/FixItSpan.cpp src/Diagnostics/FixItSpan.h src/Diagnostics/ParserErrorListener.cpp
        src/Diagnostics/ParserErrorListener.h src/utils/InitCeres.cpp src/utils/InitCeres.h src/Typing/TypeVisitor.cpp
        src/Typing/TypeVisitor.h src/Binding/Symbol.cpp src/Binding/Symbol.h src/Binding/Scope.cpp src/Binding/Scope.h
        src/Binding/BindingVisitor.cpp src/Binding/BindingVisitor.h)

##########################################
#          END MISCELLANEOUS LIBRARIES
##########################################

# SPDLog: Fast, header-only logging library
target_include_directories(compiler SYSTEM PRIVATE ${PROJECT_SOURCE_DIR}/thirdparty/spdlog/include)

##########################################
#       END MISCELLANEOUS LIBRARIES
##########################################


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

