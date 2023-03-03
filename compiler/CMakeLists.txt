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
add_executable(compiler
        src/Flow/FlowCheckVisitor.cpp
        src/Flow/FlowCheckVisitor.h
        src/Typing/Type.cpp src/Typing/Type.h
        src/Typing/TypeCheckVisitor.cpp
        src/Typing/TypeCheckVisitor.h
        src/main.cpp src/utils/log.hpp src/AST/AntlrASTGeneratorVisitor.cpp src/AST/AntlrASTGeneratorVisitor.h
        src/AST/nodes/Node.cpp src/AST/nodes/Node.h src/utils/SourceSpan.cpp src/utils/SourceSpan.h
        src/AST/nodes/CompilationUnit.cpp src/AST/nodes/CompilationUnit.h src/AST/nodes/Statements/FunctionDefinition.cpp
        src/AST/nodes/Statements/FunctionDefinition.h src/AST/nodes/Statements/VariableDeclaration.cpp src/AST/nodes/Statements/VariableDeclaration.h
        src/AST/nodes/Expressions/Expression.cpp src/AST/nodes/Expressions/Expression.h src/AST/nodes/Expressions/CommaExpression.cpp
        src/AST/nodes/Expressions/CommaExpression.h src/AST/nodes/Expressions/IntLiteralExpression.cpp src/AST/nodes/Expressions/IntLiteralExpression.h
        src/AST/nodes/Expressions/FloatLiteralExpression.cpp src/AST/nodes/Expressions/FloatLiteralExpression.h src/AST/nodes/Expressions/BoolLiteralExpression.cpp
        src/AST/nodes/Expressions/BoolLiteralExpression.h src/AST/nodes/Expressions/IdentifierExpression.cpp src/AST/nodes/Expressions/IdentifierExpression.h
        src/AST/nodes/Expressions/BinaryOperationExpression.cpp src/AST/nodes/Expressions/BinaryOperationExpression.h
        src/Typing/BinaryOperation.h src/Typing/BinaryOperation.cpp src/Typing/Constness.cpp src/Typing/Constness.h
        src/AST/nodes/Expressions/PostfixExpression.cpp src/AST/nodes/Expressions/PostfixExpression.h src/AST/nodes/Expressions/PrefixExpression.cpp
        src/AST/nodes/Expressions/PrefixExpression.h src/AST/nodes/Expressions/FunctionCallExpression.cpp src/AST/nodes/Expressions/FunctionCallExpression.h
        src/AST/nodes/Expressions/AssignmentExpression.cpp src/AST/nodes/Expressions/AssignmentExpression.h src/AST/FunctionParameter.cpp
        src/AST/FunctionParameter.h src/AST/nodes/Statements/Statement.cpp src/AST/nodes/Statements/Statement.h src/AST/nodes/Statements/BlockStatement.cpp
        src/AST/nodes/Statements/BlockStatement.h src/AST/nodes/Statements/ExpressionStatement.cpp
        src/AST/nodes/Statements/ExpressionStatement.h src/AST/nodes/Statements/ReturnStatement.cpp src/AST/nodes/Statements/ReturnStatement.h
        src/AST/nodes/Statements/IfStatement.cpp src/AST/nodes/Statements/IfStatement.h src/AST/nodes/Statements/WhileStatement.cpp
        src/AST/nodes/Statements/WhileStatement.h src/AST/nodes/Statements/ForStatement.cpp src/AST/nodes/Statements/ForStatement.h
        src/AST/AbstractASTVisitor.cpp src/AST/AbstractASTVisitor.h src/AST/ASTVisitor.cpp src/AST/ASTVisitor.h
        src/AST/AbstractReturningASTVisitor.hpp src/AST/ReturningASTVisitor.hpp src/AST/ASTStringifierVisitor.cpp
        src/AST/ASTStringifierVisitor.h src/utils/SourceManager.cpp src/utils/SourceManager.h
        src/Diagnostics/Diagnostics.def src/Diagnostics/Diagnostics.cpp src/Diagnostics/Diagnostics.h
        src/Diagnostics/FixItSpan.cpp src/Diagnostics/FixItSpan.h src/Diagnostics/ParserErrorListener.cpp
        src/Diagnostics/ParserErrorListener.h src/utils/InitCeres.cpp src/utils/InitCeres.h src/Typing/TypeVisitor.cpp
        src/Typing/TypeVisitor.h src/Binding/SymbolDeclaration.cpp src/Binding/SymbolDeclaration.h src/Binding/Scope.cpp src/Binding/Scope.h
        src/Binding/BindingVisitor.cpp src/Binding/BindingVisitor.h src/AST/nodes/FunctionDeclaration.cpp src/AST/nodes/FunctionDeclaration.h src/Typing/Visibility.cpp src/Typing/Visibility.h src/AST/nodes/Expressions/CastExpression.cpp src/AST/nodes/Expressions/CastExpression.h src/Codegen/CodegenVisitor.cpp src/Codegen/CodegenVisitor.h src/Codegen/CodeGenerator.cpp src/Codegen/CodeGenerator.h)

##########################################
#       START MISCELLANEOUS LIBRARIES
##########################################

# SPDLog: Fast, header-only logging library
target_include_directories(compiler SYSTEM PRIVATE ${PROJECT_SOURCE_DIR}/thirdparty/spdlog/include)

##########################################
#       END MISCELLANEOUS LIBRARIES
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

# Handle LLVM Targets (targets our compiler will be able to build for)
set(TARGET_WebAssembly WebAssemblyCodeGen WebAssemblyAsmParser WebAssemblyDesc WebAssemblyInfo)
set(TARGET_XCore XCoreCodeGen XCoreDesc XCoreInfo)
set(TARGET_SystemZ SystemZCodeGen SystemZAsmParser SystemZDesc SystemZInfo)
set(TARGET_Sparc SparcCodeGen SparcAsmParser SparcDesc SparcInfo)
set(TARGET_RISCV RISCVCodeGen RISCVAsmParser RISCVDesc RISCVInfo)
set(TARGET_PowerPC PowerPCCodeGen PowerPCAsmParser PowerPCDesc PowerPCInfo)
set(TARGET_NVPTX NVPTXCodeGen NVPTXDesc NVPTXInfo)
set(TARGET_MSP430 MSP430CodeGen MSP430AsmParser MSP430Desc MSP430Info)
set(TARGET_Mips MipsCodeGen MipsAsmParser MipsDesc MipsInfo)
set(TARGET_Lanai LanaiCodeGen LanaiAsmParser LanaiDesc LanaiInfo)
set(TARGET_Hexagon HexagonCodeGen HexagonAsmParser HexagonDesc HexagonInfo)
set(TARGET_BPF BPFCodeGen BPFAsmParser BPFDesc BPFInfo)
set(TARGET_ARM ARMCodeGen ARMAsmParser ARMDesc ARMUtils ARMInfo)
set(TARGET_AMDGPU AMDGPUCodeGen AMDGPUAsmParser AMDGPUDesc AMDGPUUtils AMDGPUInfo)
set(TARGET_X86 X86CodeGen X86AsmParser X86Desc X86Info)
set(TARGET_AArch64 AArch64CodeGen AArch64AsmParser AArch64Desc AArch64Utils AArch64Info)
set(TARGET_AVR AVRCodeGen AVRAsmParser AVRDesc AVRInfo)
set(TARGET_VE VECodeGen VEAsmParser VEDesc VEInfo)

# Set the list of targets we want our compiler to build for (a subset of the previously design targets)
set(TARGETS_TO_BUILD "VE" "WebAssembly" "XCore" "SystemZ" "Sparc" "RISCV" "PowerPC" "NVPTX" "MSP430" "Mips" "Lanai" "Hexagon" "BPF" "ARM" "AMDGPU" "X86" "AArch64" "AVR")

set(LLVM_TARGETS)
foreach (target IN ITEMS ${TARGETS_TO_BUILD})
    list(APPEND LLVM_TARGETS "${TARGET_${target}}")
endforeach (target)

# A list of components can be found by executing "llvm-config --components"
llvm_map_components_to_libnames(llvm_libs
        TextAPI
        OrcJIT
        JITLink
        ObjectYAML
        WindowsManifest
        Coverage
        TableGen
        LTO
        Passes
        ObjCARCOpts
        Coroutines
        LibDriver
        XRay
        ${LLVM_TARGETS}
        MIRParser
        ipo
        Instrumentation
        Vectorize
        Linker
        IRReader
        AsmParser
        Symbolize
        DebugInfoPDB
        FuzzMutate
        LineEditor
        MCA
        DebugInfoGSYM
        GlobalISel
        SelectionDAG
        AsmPrinter
        DebugInfoDWARF
        MCJIT
        Interpreter
        ExecutionEngine
        RuntimeDyld
        CodeGen
        Target
        ScalarOpts
        InstCombine
        AggressiveInstCombine
        TransformUtils
        BitWriter
        Analysis
        ProfileData
        DlltoolDriver
        Option
        Object
        MCParser
        MC
        DebugInfoCodeView
        DebugInfoMSF
        BitReader
        Core
        Remarks
        BinaryFormat
        BitstreamReader
        Support
        Demangle
        )

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

