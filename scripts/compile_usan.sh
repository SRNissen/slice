#!/bin/sh
# This is a comment!

echo "script: compile_usan.sh"

cd src
    for F in *.c; do
        zig cc -g -fsanitize=undefined -Werror -Wall -Wextra -Weverything -c $F -o ../san/usan/obj/${F%.c}.o
    done
cd ..

cd san
    cd usan
        cd obj
            zig cc -g -fsanitize=undefined -Werror -Wall -Wextra -Weverything * -o ../bin/test
        cd ..
    cd ..
cd ..