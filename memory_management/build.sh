#!/bin/sh

set -xe

zig cc -Wall -Wextra -o a.out main.c

./a.out test arg1 arg2 arg3
