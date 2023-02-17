#!/bin/sh
# This is a comment!

echo
echo "script: compile_msan.sh"

cd src
    for F in *.c; do
        clang -g -fsanitize=address -Werror -Wall -Wextra -Weverything -c $F -o ../san/msan/obj/${F%.c}.o
    done
cd ..

cd san
    cd msan
        cd obj
            clang -g -fsanitize=address -fsanitize=memory -Werror -Wall -Wextra -Weverything * -o ../bin/test
        cd ..
    cd ..
cd ..