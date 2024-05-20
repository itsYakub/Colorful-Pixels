<div align="Center">

# Colorful Pixels 🎨

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Windows](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![WebGL](https://img.shields.io/badge/WebGL-990000?logo=webgl&logoColor=white&style=for-the-badge)
[![GitHub](https://img.shields.io/badge/github-%23121011.svg?style=for-the-badge&logo=github&logoColor=white)](https://github.com/itsYakub/Colorful-Pixels)
[![Itch.io](https://img.shields.io/badge/Itch-%23FF0B34.svg?style=for-the-badge&logo=Itch.io&logoColor=white)](https://itsyakub.itch.io/colorful-pixels)
[![Ko-Fi](https://img.shields.io/badge/Ko--fi-F16061?style=for-the-badge&logo=ko-fi&logoColor=white)](https://ko-fi.com/yakub)

![](res/images/colorful_pixels_screenshot.png)

</div>

## Overview:
**Colorful Pixels** is a simple and easy-to-use pixel-art editor.

Software supports all range of useful features, designed to make your life easy and comfortable:
- Clean User-Interface with light and dark theme. 
- Full range of tools to draw, delete and manipulate your creations.
- Layer system with options to preview, lock, enable visibility and move every layer up and down.
- Versatile color manipulation.

## Installation:
### 0. Prerequesites:
- **[Git;](https://git-scm.com/)**
- **[CMake;](https://cmake.org/)**
- **[Build system of your choosing;](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html#id7)**
### 1. Manual setup:
- Clone this repository:
```console
$ git clone https://github.com/itsYakub/Colorful-Pixels.git --depth=1 --branch=alpha1.0
$ cd Colorful-Pixels
```
- Create a binary folder:
```console
$ mkdir bin/
$ cd bin/
```
- Create the generator file and build the final executable:
```console
cmake .. -DPLATFORM=Desktop
cmake --build .
```
- Run the executable:
```console
./colorful_pixels
```

## Credits:
- **[Emscripten.](https://github.com/emscripten-core/emsdk.git)**
- **[raylib.](https://github.com/raysan5/raylib.git)**
- **[rlImGui.](https://github.com/raylib-extras/rlImGui.git)**
- **[imgui.](https://github.com/ocornut/imgui.git)**
- **[tinyfiledialogs.](https://github.com/native-toolkit/libtinyfiledialogs.git)**
- **[json.](https://github.com/nlohmann/json.git)**

## Licence:
This template is under the **[MIT Licence](https://github.com/itsYakub/Colorful-Pixels/blob/main/LICENCE)**.

<!-- Badges: https://github.com/Ileriayo/markdown-badges -->