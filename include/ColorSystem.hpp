#pragma once

#include "raylib.h"
#include "imgui.h"

#include "ColorPalette.hpp"

class ColorSystem {
private:
    ColorPalette m_ColorPalette;
    Color m_CurrentColor;

public:
    ColorSystem();

    ColorPalette& GetPalette();
    void SetPalette(ColorPalette& newPalette);

    Color& GetColor();
    void SetColor(Color newColor);
    void SetColor(float newColor[4]);

    void ColorGuiPanel(const char* name, bool draw);
};