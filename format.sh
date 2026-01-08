#!/bin/sh
clang-format --verbose -i `find engine/src engine/include engine/net engine/tools -name "*.c" -or -name "*.h"`
