#!/bin/sh
# This is a comment!

echo
echo "script: compile_usan.sh"

cd src
    for F in *.c; do
        clang -g -fsanitize=undefined -Werror -Wall -Wextra -Weverything -c $F -o ../san/usan/obj/${F%.c}.o
    done
cd ..

cd san
    cd usan
        cd obj
            clang -g -fsanitize=undefined -Werror -Wall -Wextra -Weverything * -o ../bin/test
        cd ..
    cd ..
cd ..