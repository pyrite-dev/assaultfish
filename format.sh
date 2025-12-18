#!/bin/sh
clang-format --verbose -i `find include server client -name "*.c" -or -name "*.h"`
