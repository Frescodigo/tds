#!/bin/bash

set -xe

CFLAGS="-Wall -Wextra -ggdb"
INCLUDES="-Iinclude"
LIBS="-Llib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11"

gcc $CFLAGS -o build/game.so $INCLUDES $LIBS src/game.c -shared -fPIC

exit 0

gcc $CFLAGS -o game src/game.c build.so 
