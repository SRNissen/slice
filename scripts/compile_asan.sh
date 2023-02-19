#!/bin/sh
# This is a comment!

echo "script: compile_asan.sh"

cd src
    for F in *.c; do
        clang -g -fsanitize=address -Werror -Wall -Wextra -Weverything -c $F -o ../san/asan/obj/${F%.c}.o
    done
cd ..

cd san
    cd asan
        cd obj
            clang -g -fsanitize=address -Werror -Wall -Wextra -Weverything * -o ../bin/test
        cd ..
    cd ..
cd ..