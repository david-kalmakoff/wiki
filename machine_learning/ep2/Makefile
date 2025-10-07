run:
	./build/nn

.PHONY: build
build:
	mkdir -p build
	zig cc -Wall -Wextra -o build/nn src/nn.c -lm

build-run: build run

debug: build
	gdb --args ./build/nn
