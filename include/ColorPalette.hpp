#pragma once

#include <vector>

#include "raylib.h"
#include "raymath.h"

class ColorPalette {
private:
    std::vector<Color> m_ColorList;

public:
    ColorPalette();
    
    int Size();
    Color GetColor(int index);
};