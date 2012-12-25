#!/bin/sh
gcc -o interceptor src/interceptor.c src/main_popen2.c -lutil

