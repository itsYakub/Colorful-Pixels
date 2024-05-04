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
    int width;
    int height;

    Viewport& viewport;
    ColorSystem& colorSystem;
    ToolSystem& toolSystem;

    std::unique_ptr<Canvas> canvas;
    std::unique_ptr<Tool> tool;
    
    Camera2D camera;

    bool valid;
    
public:
    Project(Viewport& viewport, ColorSystem& colorSystem, ToolSystem& toolSystem);

    void Load(std::string title, int width, int height);
    void Unload();
    void Update();
    void Render();

    void Pan();
    void Zoom();
};