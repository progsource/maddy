#!/bin/bash -eu

# Copy all fuzzer executables to $OUT/
$CXX $CFLAGS $LIB_FUZZING_ENGINE \
  $SRC/maddy/.clusterfuzzlite/parser_fuzzer.cpp \
  -o $OUT/parser_fuzzer \
  -I$SRC/maddy/include
