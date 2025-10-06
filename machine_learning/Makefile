run:
	./build/gates

.PHONY: build
build:
	mkdir -p build
	zig cc -Wall -Wextra -o build/gates src/gates.c -lm
	zig cc -Wall -Wextra -o build/twice src/twice.c -lm

build-run: build run
