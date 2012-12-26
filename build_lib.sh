#!/bin/sh
if [ ! -d target/build ]; then
	mkdir -p target/build
fi

gcc -c -fPIC src/interceptor.c -o target/build/interceptor.o -shared -lutil

if [ ! -d target/release ]; then
	mkdir -p target/release
fi

gcc -shared -Wl -o target/release/libinterceptor.so.1.0.1 target/build/interceptor.o -lutil

