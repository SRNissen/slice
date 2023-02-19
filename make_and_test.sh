#!/bin/sh

echo
echo "script: make_and_test.sh"
echo
./scripts/clean.sh
echo
./scripts/compile_asan.sh
./scripts/test_asan.sh
echo
./scripts/compile_msan.sh
./scripts/test_msan.sh
echo
./scripts/compile_usan.sh
./scripts/test_usan.sh
echo
./scripts/compile.sh
./scripts/test.sh