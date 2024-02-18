#!/bin/bash

imgui_url="https://github.com/ocornut/imgui.git"
googletest_url="https://github.com/google/googletest.git"

echo -e "Pulling external dependencies.\n"

git submodule add $imgui_url external/imgui

git submodule add $googletest_url external/googletest

echo -e "Creating build directory.\n"
mkdir build && cd build

echo -e "Creating release directory.\n"
mkdir release && cd release

echo -e "Generating Release type build files.\n"

cmake -DCMAKE_BUILD_TYPE=Release ../..

echo -e "Compiling program.\n"

make
