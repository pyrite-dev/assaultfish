#!/bin/sh
clang-format --verbose -i `find include server client tools src -name "*.c" -or -name "*.h"`
