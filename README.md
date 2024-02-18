# Interactive Code-Smell Visualization for Large Codebases

## Requirements

- CMake 3.13.0+
- Ogre v13.4.4 <https://github.com/OGRECave/ogre/releases/tag/v13.4.4>
- JsonCpp <https://github.com/open-source-parsers/jsoncpp>

## Installing Ogre

Using the CMake interface to configure the project make sure to Enable the following components:

- COMPONENT_BITES
- COMPONENT_OVERLAY
- COMPONENT_OVERLAY_IMGUI

Also make sure **OGRE_STATIC** is **DISABLED**.

After generating the project navigate into the build directory and run following commands:
- `make`
- `make install`

## Installing ICSV

Clone the project:
- `git clone https://github.com/VakrasVasileios/ICSV.git`
- `cd ICSV`

Run the appropriate script, after changing its execution policy, if needed.
#### Linux
- `chmod +x linux_build.sh`
- `./linux_build.sh`

#### Windows
- `windows_build.bat`

Or execute the following commands:

- `mkdir build`

#### Release mode
- `cd build`
- `mkdir release & cd release`
- `cmake -DCMAKE_BUILD_TYPE=Release ../..`
- `make`

#### Debug mode
- `cd build`
- `mkdir debug & cd debug`
- `cmake -DCMAKE_BUILD_TYPE=Debug ../..`
- `make`

#### Run the executable
- `./build/<debug/release>/ICSV-app/src/ICSVapp`
