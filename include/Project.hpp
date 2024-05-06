#pragma once

#include <memory>
#include <string>

#include "raylib.h"
#include "nlohmann/json.hpp"

#include "LayerSystem.hpp"
#include "Canvas.hpp"
#include "Tool.hpp"

class Canvas;
class Viewport;
class ColorSystem;
class ToolSystem;

class Project {
public:
    std::string title;

    Viewport& viewport;
    ColorSystem& colorSystem;
    ToolSystem& toolSystem;

    std::unique_ptr<Canvas> canvas;
    std::unique_ptr<Tool> tool;
    
    Camera2D camera;

    bool valid;
    bool modified;
    
public:
    Project(Viewport& viewport, ColorSystem& colorSystem, ToolSystem& toolSystem);

    void Load(std::string title, int width, int height);

    void IOSave(nlohmann::json& json);
    void IOLoad(nlohmann::json& json);
    
    void Unload();
    void Update();
    void Render();

    void Pan();
    void Zoom();
};