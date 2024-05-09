#include "Project.hpp"

#include <cstdint>
#include <cstring>
#include <math.h>
#include <memory>

#include "LayerSystem.hpp"
#include "raylib.h"
#include "raymath.h"

#include "Canvas.hpp"
#include "Viewport.hpp"
#include "ColorSystem.hpp"
#include "ToolSystem.hpp"
#include "ColorfulPixels.hpp"

Project::Project(Viewport& viewport, ColorSystem& colorSystem, ToolSystem& toolSystem) :
    title(),
    valid(false),
    modified(false),
    viewport(viewport),
    colorSystem(colorSystem),
    toolSystem(toolSystem) { }

void Project::Create(int width, int height) {
    title = "sprite01";

    canvas = std::make_unique<Canvas>(width, height);
    tool = toolSystem.SetCurrentTool(ToolSystem::TOOL_BRUSH, *this);

    camera.target = Vector2Zero();
    camera.offset = { 512.0f, 128.0f };
    camera.zoom = canvas->scale;

    valid = canvas.get() && tool.get();
    savedOnDisk = false;
}

void Project::Serialize(nlohmann::json& json) {
    json["cfpx_version"] = COLORFUL_PIXELS_VERSION;
    json["cfpx_version_major"] = COLORFUL_PIXELS_VERSION_MAJOR;
    json["cfpx_version_minor"] = COLORFUL_PIXELS_VERSION_MINOR;

    json["cfpx_project_canvas_width"] = canvas->CELL_COUNT_X;
    json["cfpx_project_canvas_height"] = canvas->CELL_COUNT_Y;
    json["cfpx_project_canvas_scale"] = canvas->scale;

    LayerSystem& layerSystem = canvas->layerSystem;

    json["cfpx_project_layer_count"] = layerSystem.GetList().size();

    for(int layerIndex = 0; layerIndex < layerSystem.GetList().size(); layerIndex++) {
        json["cfpx_project_layers"][layerIndex]["cfpx_project_layer_id"] = layerSystem.GetLayer(layerIndex).GetID();
        json["cfpx_project_layers"][layerIndex]["cfpx_project_layer_visible"] = layerSystem.GetLayer(layerIndex).layerVisible;
        json["cfpx_project_layers"][layerIndex]["cfpx_project_layer_locked"] = layerSystem.GetLayer(layerIndex).layerLocked;

        int cellCount = layerSystem.GetLayer(layerIndex).GetSizeX() * layerSystem.GetLayer(layerIndex).GetSizeX();
        for(int cellColor = 0; cellColor < cellCount; cellColor++) {
            json["cfpx_project_layers"][layerIndex]["cfpx_project_layer_data"][cellColor] = ColorToInt(layerSystem.GetLayer(layerIndex).GetPixelColor(cellColor));
        }
    }

    json["cfpx_project_camera_target"] = { camera.target.x, camera.target.y };
    json["cfpx_project_camera_offset"] = { camera.offset.x, camera.offset.y };
    json["cfpx_project_camera_zoom"] = camera.zoom;

    json["cfpx_project_current_tool_id"] = tool->ID;

    this->modified = false;
    this->savedOnDisk = true;
}

void Project::Deserialize(nlohmann::json& json) {
    std::string version = json["cfpx_version"];
    strcmp(COLORFUL_PIXELS_VERSION, version.c_str()) == 0 ?
        TraceLog(LOG_INFO, TextFormat("PROJECT: Loading the project for version: %s", version.c_str())) :
        TraceLog(LOG_WARNING, TextFormat("PROJECT: Loading the project for an obsolete version of Colorful Pixels: %s (Current version: %s)", version.c_str(), COLORFUL_PIXELS_VERSION));

    int cellCountX = json["cfpx_project_canvas_width"];
    int cellCountY = json["cfpx_project_canvas_height"];

    LayerSystem layerSystem = LayerSystem(cellCountX, cellCountY, false);

    int layerCount = json["cfpx_project_layer_count"];
    for(int layerIndex = 0; layerIndex < layerCount; layerIndex++) {
        layerSystem.PushNewLayer(
            cellCountX, 
            cellCountY,
            json["cfpx_project_layers"][layerIndex]["cfpx_project_layer_id"],
            json["cfpx_project_layers"][layerIndex]["cfpx_project_layer_visible"],
            json["cfpx_project_layers"][layerIndex]["cfpx_project_layer_locked"]
        );

        int cellCount = cellCountX * cellCountY;
        for(int cellColor = 0; cellColor < cellCount; cellColor++) {
            Color color = GetColor(json["cfpx_project_layers"][layerIndex]["cfpx_project_layer_data"][cellColor]);

            layerSystem.GetLayer(layerIndex).SetPixelColor(
                cellColor, 
                color
            );
        }
    }

    this->camera.offset = { 
        json["cfpx_project_camera_offset"][0],
        json["cfpx_project_camera_offset"][1]
    };

    this->camera.target = { 
        json["cfpx_project_camera_target"][0],
        json["cfpx_project_camera_target"][1]
    };

    this->camera.zoom = json["cfpx_project_camera_zoom"];

    this->canvas = std::make_unique<Canvas>(
        cellCountX,
        cellCountY,
        layerSystem
    );

    this->canvas->scale = json["cfpx_project_canvas_scale"];

    this->tool = toolSystem.SetCurrentTool(
        static_cast<ToolSystem::ToolList>(json["cfpx_project_current_tool_id"]), 
        *this
    );

    this->canvas->ToggleCanvasReload();
    this->valid = canvas.get() && tool.get();
    this->savedOnDisk = true;
}

void Project::Unload() {
    viewport.Unload();

    if(valid) {
        canvas->Unload();
    }

    valid = false;
}

void Project::Update() {
    if(!valid) {
        return;
    }

    // Check if mouse cursor is placed on the viewport...
    if(viewport.IsCursorInViewport()) {        
        // ...Check if left mouse button is pressed
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            tool->OnButtonPress();
        }

        // ...Check if left mouse button is down
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            tool->OnButtonDown();
        }

        // ...Check if left mouse button is released
        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            tool->OnButtonRelease();
        }

        // ...Check if scroll is pressed
        if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT) || IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
            Pan();
        }

        // ...Check if scroll accelerates
        if(GetMouseWheelMove() != 0.0f) {
            Zoom();
        }
    }

    canvas->Update(camera, viewport.GetPosition());
}

void Project::Render() {
    BeginMode2D(camera);

        if(valid) {
            canvas->Render(camera, viewport.GetPosition(), colorSystem);
            tool->Render();
        }

    EndMode2D();
}

void Project::Pan() {
    Vector2 delta = GetMouseDelta();
    delta = Vector2Scale(delta, -1.0f / camera.zoom);

    camera.target = Vector2Add(camera.target, delta);
    camera.target.x = round(camera.target.x);
    camera.target.y = round(camera.target.y);
}

void Project::Zoom() {
    canvas->scale += GetMouseWheelMove() / 10.0f;
    canvas->scale = Clamp(canvas->scale, 0.5f, 4.0f);
    
    camera.target = GetScreenToWorld2D(GetMousePosition(), camera);
    camera.offset = GetMousePosition();
    camera.zoom = canvas->scale;
}

