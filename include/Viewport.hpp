#pragma once

#include "Cursor.hpp"
#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"

class Cursor;

class Viewport {
private:
    RenderTexture2D m_RenderTexture;

    Vector2 m_Position;
    Vector2 m_Size;

public:
    Viewport();

    void Load();
    void Unload();

    void ViewportGuiPanel(const char* name, bool draw);

    void Begin();
    void Clear(Color color);
    void End();

    Vector2 GetPosition();
    Vector2 GetSize();
    Vector2 GetTextureSize();
    Vector2 GetOffset();
    Vector2 GetPositionOffset();


    bool IsCursorInViewport();
};