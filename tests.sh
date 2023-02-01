#!/bin/bash
diff <(./compiler/cmake-build/compiler tests/parser/001.functions.slc) tests/parser/001.functions.out
