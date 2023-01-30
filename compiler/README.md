# SLC Compiler

The SLC Compiler uses ANTLR4 as its lexer and parser generator. It uses LLVM to generate code. It also uses CMake as its build system.

## Prerrequisites

- `Java`: Java 11 or later needed.
- `llvm-dev`: LLVM 15 with static libraries and includes must be installed.
- `git`
- `cmake`

## Installing llvm-15-dev

### Ubuntu and Debian

`llvm-15-dev` may not be in the official repositories yet. To install it, you can use [https://apt.llvm.org/](https://apt.llvm.org/). 

To install `llvm-15` using their automatic installation script run

```
wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh
sudo ./llvm.sh 15
```

### Arch

You can install install `llvm-15` from the official repositories. Currently, the `llvm` package in the `extra` repo is points to `llvm-15`. If it has been updated to a more recent version, you can search the AUR.

### Windows

You can follow the instructions on [https://llvm.org/docs/GettingStartedVS.html](https://llvm.org/docs/GettingStartedVS.html) to build LLVM from source.

Note that you may have to build LLVM twice if you want to execute SLC compiler as both Release and Debug, or you will find MSVC Linker errors regarding MSRT mismatch. Also note that the debug version of LLVM is quite heavy, around 70 gigabytes. You can also compile SLC in RelWithDebInfo and link against the Release LLVM libraries. 

### Building from source

You can install LLVM from source following the instructions at [https://llvm.org/docs/GettingStarted.html](https://llvm.org/docs/GettingStarted.html).

## Building

If after following this section you get LLVM not detected errors from CMake, follow the section CMAKE_PREFIX_PATH.

Once all the prerrequisites have been installed, execute the following commands from the `compiler` folder to build the compiler:
```
mkdir cmake-build
cd cmake-build
cmake ..
cmake --build .
```

### CMAKE_PREFIX_PATH

CMake has not been able to detect LLVM in the default paths, so you have to point the folder where LLVM is installed directly to it.
Execute the following commands from the `compiler` folder:

```
mkdir cmake-build
cd cmake-build
cmake -DCMAKE_PREFIX_PATH=/path/to/llvm ..
cmake --build .
```
