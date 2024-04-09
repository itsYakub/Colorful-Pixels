<div align="Center">

# Raylib Game Template 🎮

</div>

## Overview:
This is the general-purpose raylib project template in C++.

## Prerequesites:
- **[Git;](https://git-scm.com/)**
- **[CMake;](https://cmake.org/)**
- **[Build system of your choosing;](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html#id7)**

## Naming convention:
This list presents the naming convention I'm using for my project structure. Feel free to modify it, but remember to also make simillar changes in the **[CMakeLists.txt](https://github.com/itsYakub/RaylibGameTemplate/blob/main/CMakeLists.txt)**.
- **Source directory** - *src*
- **Include directory** - *include*
- **Dependencies / Modules / Libraries directory** - *lib*
- **Resources directory** - *res*
- **Binary / Build directory** - *bin*

## Setup
### 1. Build For Desktop:
- Pull all the necessary **submodules**:
```bash
git submodule update --init
```
- Create a **binary diretory**:
```bash
mkdir bin
cd bin
```
- Use the **following command**:
```cmake
cmake ..
```
### 2. Build For WEB:
- Pull all the necessary **submodules**:
```bash
git submodule update --init
```
- Go to the **emscripten submodule**:
```bash
cd lib/emsdk
```
- **Install and activate** the emsdk following the **[Installation Instruction](https://emscripten.org/docs/getting_started/downloads.html#installation-instructions-using-the-emsdk-recommended)**:
```bash
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
cd ../..
```
- Create a **binary diretory**:
```bash
mkdir bin
cd bin
```
- Use the **following commands**:
```bash
emcmake cmake .. -DPLATFORM=Web
emmake make
```
*(***WARNING:*** if those commands returns an error, make sure that your project is set correctly: ***directory relations are correct, path to the shell file is correct, path to the resources is correct, resources directory exists etc.***)*
- To **test** your final executable (*.html, *.json, *.wasm, *.data, *.mem) you can use the **following command**:
```bash
emrun ./{game_title}.html
```
***NOTE:*** *This CMake workflow is managed from the position of the binary directory; if you want to use CMake from the main project's directory, modify the CMake file respectively (mainly: `--shell-file` and `--preload-file`)*.

## Credits
- **[Raylib CMake project](https://github.com/raysan5/raylib/tree/master/projects/CMake)**
- **[github/gitignore](https://github.com/github/gitignore)**
- **[gitattributes/gitattributes](https://github.com/gitattributes/gitattributes)**
- **[licenses/license-templates](https://github.com/licenses/license-templates)**

## Licence
This template is originally under the **[MIT Licence](https://github.com/itsYakub/RaylibGameTemplate/blob/main/LICENCE)**, but you can modify it to suite your needs. You can use **[licenses/license-templates](https://github.com/licenses/license-templates)** to get the free licence template.