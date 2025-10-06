#!/bin/bash

mkdir -p ./build
pushd ./build

c++ ../code/sdl_handmade.cpp \
	../libs/logging/*.c \
	-o handmadehero \
	`sdl2-config --cflags --libs` -g
popd
