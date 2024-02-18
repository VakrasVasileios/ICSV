
SET imgui_url=https://github.com/ocornut/imgui.git
SET googletest_url=https://github.com/google/googletest.git
echo "-e" "Pulling external dependencies.
"
git "submodule" "add" "%imgui_url%" "external\imgui"
git "submodule" "add" "%googletest_url%" "external\googletest"
echo "-e" "Creating build directory.
"
mkdir "build" && cd "build"
echo "-e" "Creating release directory.
"
mkdir "release" && cd "release"
echo "-e" "Generating Release type build files.
"
cmake "-DCMAKE_BUILD_TYPE=Release" "%CD%\..\.."
echo "-e" "Compiling program.
"
make