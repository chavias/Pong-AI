#!/bin/sh

# cmake -S . -B build/
cmake -G Ninja -DCUSTOMIZE_BUILD=OFF -DCMAKE_BUILD_TYPE=DEBUG -S . -B build/