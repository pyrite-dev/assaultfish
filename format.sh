#!/bin/sh
clang-format --verbose -i `find include server client tools -name "*.c" -or -name "*.h"`
