#!/bin/sh

echo
echo "script: make_and_test.sh"

./scripts/clean.sh

./scripts/compile_asan.sh
./scripts/test_asan.sh

./scripts/compile_msan.sh
./scripts/test_msan.sh

./scripts/compile.sh
./scripts/test.sh