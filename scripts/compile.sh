#!/bin/sh
# This is a comment!

echo "script: compile.sh"

cd src
    for F in *.c; do
        zig cc -Werror -Wall -Wextra -c $F -o ../obj/${F%.c}.o
    done
cd ..

cd obj
    zig cc -Werror -Wall -Wextra  * -o ../bin/test
cd ..