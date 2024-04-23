#include "Viewport.hpp"

#include "imgui.h"
#include "raylib.h"
#include "raymath.h"
#include "rlImGui.h"

Viewport::Viewport() :
    m_RenderTexture(LoadRenderTexture(2048, 2048)),
    m_Position(Vector2Zero()), m_Size(Vector2Zero()) { }

void Viewport::Unload() {
    UnloadRenderTexture(m_RenderTexture);
}

void Viewport::ViewportGuiPanel(const char* name, bool draw) {
    if(!draw) {
        return;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin(
        name, 
        nullptr,
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse
    );

    m_Position.x = ImGui::GetWindowPos().x;
    m_Position.y = ImGui::GetWindowPos().y;

    m_Size.x = ImGui::GetWindowSize().x;
    m_Size.y = ImGui::GetWindowSize().y;

    rlImGuiImageRenderTexture(&m_RenderTexture);

    ImGui::PopStyleVar(1);

    ImGui::End();
}

void Viewport::Begin() {
    BeginTextureMode(m_RenderTexture);
}

void Viewport::Clear(Color color) {
    ClearBackground(color);
}

void Viewport::End() {
    EndTextureMode();
}

Vector2 Viewport::GetPosition() {
    return m_Position;
}

Vector2 Viewport::GetSize() {
    return m_Size;
}

Vector2 Viewport::GetTextureSize(){
    return {
        static_cast<float>(m_RenderTexture.texture.width),
        static_cast<float>(m_RenderTexture.texture.height)
    };
}

Vector2 Viewport::GetOffset() {
    ImGuiStyle& style = ImGui::GetStyle();

    return {
        style.WindowPadding.x,
        style.WindowPadding.y
    };
}

Vector2 Viewport::GetPositionOffset() {
    return {
        GetPosition().x + GetOffset().x,
        GetPosition().y + GetOffset().y
    };
}

bool Viewport::IsCursorInViewport() {
    return CheckCollisionPointRec(GetMousePosition(), (Rectangle) { m_Position.x, m_Position.y, m_Size.x, m_Size.y } );
}
