#!/bin/sh

# cmake -S . -B build/
rm -rf build/
cmake -G Ninja -DCUSTOMIZE_BUILD=OFF -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=RELEASE -S . -B build/