#!/bin/sh
clang-format --verbose -i `find engine/common engine/net engine/tools -name "*.c" -or -name "*.h"`
