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

You can install LLVM using Chocolatey [https://community.chocolatey.org/packages/llvm](https://community.chocolatey.org/packages/llvm).



### Building from source

You can install LLVM from source following the instructions at [https://llvm.org/docs/GettingStarted.html](https://llvm.org/docs/GettingStarted.html).

Run in the command line interpreter of powershell

```
choco install llvm --version 15
```

## Building

Once all the prerrequisites have been installed, execute the following commands from the `compiler` folder to build the compiler:
```
mkdir cmake-build
cd cmake-build
cmake ..
cmake --build .
```