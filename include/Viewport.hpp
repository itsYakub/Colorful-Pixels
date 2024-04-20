#pragma once

#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"

class Viewport {
private:
    RenderTexture2D m_RenderTexture;

    Vector2 m_Position;
    Vector2 m_Size;

public:
    Viewport(const Vector2 position, const Vector2 size);
    void Unload();

    void ViewportGuiPanel(const char* name, ImVec2 position, ImVec2 size);

    void Begin();
    void Clear(Color color);
    void End();

    Vector2 GetPosition();
    Vector2 GetSize();
};