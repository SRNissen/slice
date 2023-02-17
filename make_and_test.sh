#!/bin/sh

echo
echo "script: make_and_test.sh"

./clean.sh

./compile_asan.sh
./test_asan.sh

./compile_msan.sh
./test_msan.sh

./compile.sh
./test.sh