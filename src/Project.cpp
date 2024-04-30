#include "Project.hpp"

#include <memory>

#include "raylib.h"
#include "raymath.h"

#include "Canvas.hpp"
#include "Viewport.hpp"
#include "ColorSystem.hpp"
#include "ToolSystem.hpp"

Project::Project(Viewport& viewport, ColorSystem& colorSystem, ToolSystem& toolSystem) :
    m_Viewport(viewport),
    m_ColorSystem(colorSystem),
    m_ToolSystem(toolSystem) { }

void Project::Load() {
    canvas = std::make_unique<Canvas>(&m_Viewport, 16);
    tool = m_ToolSystem.SetCurrentTool(ToolSystem::TOOL_BRUSH, *this, m_ColorSystem);

    camera.target = Vector2Zero();
    camera.offset = { 512.0f, 128.0f };
    camera.zoom = canvas->scale;
}

void Project::Unload() {
    m_Viewport.Unload();
    canvas->Unload();
}

void Project::Update() {
    // Check if mouse cursor is placed on the viewport...
    if(m_Viewport.IsCursorInViewport()) {        
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

    canvas->Update();
}

void Project::Render() {
    BeginMode2D(camera);

        canvas->Render(camera);
        tool->Render();

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
