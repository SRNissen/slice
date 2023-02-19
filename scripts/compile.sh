#!/bin/sh
# This is a comment!

echo "script: compile.sh"

cd src
    for F in *.c; do
        gcc -Werror -Wall -Wextra -c $F -o ../obj/${F%.c}.o
    done
cd ..

cd obj
    gcc -Werror -Wall -Wextra  * -o ../bin/test
cd ..