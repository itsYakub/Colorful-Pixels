#include "Project.hpp"

#include <memory>

#include "raylib.h"
#include "raymath.h"

#include "Canvas.hpp"
#include "Viewport.hpp"
#include "ColorSystem.hpp"
#include "ToolSystem.hpp"

Project::Project(Viewport& viewport, ColorSystem& colorSystem, ToolSystem& toolSystem) :
    title(),
    width(32),
    height(32),
    valid(false),
    viewport(viewport),
    colorSystem(colorSystem),
    toolSystem(toolSystem) { }

void Project::Load(std::string title, int width, int height) {
    this->title = title;

    canvas = std::make_unique<Canvas>(width, height);
    tool = toolSystem.SetCurrentTool(ToolSystem::TOOL_BRUSH, *this);

    camera.target = Vector2Zero();
    camera.offset = { 512.0f, 128.0f };
    camera.zoom = canvas->scale;

    valid = canvas.get() && tool.get();
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
            canvas->Render(camera, viewport.GetPosition());
            tool->Render();
        }

    EndMode2D();
}

void Project::Pan() {
    Vector2 delta = GetMouseDelta();
    delta = Vector2Scale(delta, -1.0f / camera.zoom);

    camera.target = Vector2Add(camera.target, delta);
}

void Project::Zoom() {
    canvas->scale += GetMouseWheelMove() / 10.0f;
    canvas->scale = Clamp(canvas->scale, 0.5f, 4.0f);
    
    camera.target = GetScreenToWorld2D(GetMousePosition(), camera);
    camera.offset = GetMousePosition();
    camera.zoom = canvas->scale;
}

