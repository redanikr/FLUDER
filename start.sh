#!/bin/bash
cmake . -GNinja -Bbuild
ninja -C ./build
./build/FLUDER