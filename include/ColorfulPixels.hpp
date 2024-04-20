#pragma once

#include <memory>

#include "raylib.h"

#include "Viewport.hpp"
#include "Canvas.hpp"

class ColorfulPixels {
private:
    std::unique_ptr<Viewport> m_Viewport;
    std::unique_ptr<Canvas> m_Canvas;

public:
    ColorfulPixels();

    void Load();
    void Unload();

    void Update();
    void Render();
    void RenderGUI();
};