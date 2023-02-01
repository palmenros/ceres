#!/bin/bash
diff <(./compiler/cmake-build/compiler tests/parser/001.functions.crs) tests/parser/001.functions.out
