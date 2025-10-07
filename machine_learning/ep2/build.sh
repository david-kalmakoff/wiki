#!/bin/sh

mkdir -p build

set -xe

gcc -Wall -Wextra -o build/nn src/nn.c -lm
