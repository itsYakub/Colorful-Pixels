#pragma once

#include <memory>

#include "raylib.h"

#include "Canvas.hpp"
#include "Tool.hpp"

class Canvas;
class Viewport;
class ColorSystem;
class ToolSystem;

class Project {
public:
    std::unique_ptr<Canvas> canvas;
    std::unique_ptr<Tool> tool;
    
    Camera2D camera;

private:
    Viewport& m_Viewport;
    ColorSystem& m_ColorSystem;
    ToolSystem& m_ToolSystem;

public:
    Project(Viewport& viewport, ColorSystem& colorSystem, ToolSystem& toolSystem);

    void Load();
    void Unload();
    void Update();
    void Render();

    void Pan();
    void Zoom();
};