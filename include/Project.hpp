#pragma once

#include <memory>
#include <string>

#include "LayerSystem.hpp"
#include "raylib.h"

#include "Canvas.hpp"
#include "Tool.hpp"

class Canvas;
class Viewport;
class ColorSystem;
class ToolSystem;

class Project {
public:
    std::string title;
    std::string path;
    int width;
    int height;

    std::unique_ptr<Canvas> canvas;
    std::unique_ptr<Tool> tool;
    
    Camera2D camera;

    bool valid;

private:
    Viewport& m_Viewport;
    ColorSystem& m_ColorSystem;
    ToolSystem& m_ToolSystem;

public:
    Project(Viewport& viewport, ColorSystem& colorSystem, ToolSystem& toolSystem);

    void Load(std::string title, std::string path, int width, int height);
    void Unload();
    void Update();
    void Render();

    void Pan();
    void Zoom();
};