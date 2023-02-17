#!/bin/sh
# This is a comment!

cd src
    for F in *.c; do
        gcc -c $F -o ../obj/${F%.c}.o
    done
cd ..

cd obj
    gcc * -o ../bin/test
cd ..