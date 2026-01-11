#!/bin/sh
clang-format --verbose -i `find engine/src engine/include engine/net engine/tools '(' -name "*.c" -or -name "*.h" ')' -and -not -name "stb_*.h" | sed 's%engine/include/GearSrc/GL/GL.h%%g'`
