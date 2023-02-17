#!/bin/sh
# This is a comment!

echo
echo "script: clean.sh"

cd obj
    touch cleaningdummy
    rm *
cd ..

cd bin
    touch cleaningdummy
    rm *
cd ..

cd san

    cd asan

        cd bin
            touch cleaningdummy
            rm *
        cd ..

        cd obj
            touch cleaningdummy
            rm *
        cd ..
    cd ..
    
    cd msan

        cd bin
            touch cleaningdummy
            rm *
        cd ..

        cd obj
            touch cleaningdummy
            rm *
        cd ..

    cd ..
    
cd ..