#!/bin/sh
# This is a comment!

echo "script: clean.sh"

mkdir -p obj
cd obj
    touch cleaningdummy
    rm *
cd ..

mkdir -p bin
cd bin
    touch cleaningdummy
    rm *
cd ..

mkdir -p san
cd san

    mkdir -p asan
    cd asan

        mkdir -p bin
        cd bin
            touch cleaningdummy
            rm *
        cd ..

        mkdir -p obj
        cd obj
            touch cleaningdummy
            rm *
        cd ..
    cd ..
    
    mkdir -p msan
    cd msan

        mkdir -p bin
        cd bin
            touch cleaningdummy
            rm *
        cd ..

        mkdir -p obj
        cd obj
            touch cleaningdummy
            rm *
        cd ..

    cd ..
    
        mkdir -p usan

        cd usan
        mkdir -p bin
        cd bin
            touch cleaningdummy
            rm *
        cd ..

        mkdir -p obj
        cd obj
            touch cleaningdummy
            rm *
        cd ..

    cd ..
    
cd ..