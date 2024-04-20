#include "Viewport.hpp"

Viewport::Viewport(const Vector2 position, const Vector2 size) :
    m_RenderTexture(LoadRenderTexture(size.x, size.y)),
    m_Position(position), m_Size(size) { }

void Viewport::Unload() {
    UnloadRenderTexture(m_RenderTexture);
}

void Viewport::ViewportGuiPanel(const char* name, ImVec2 position, ImVec2 size) {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin(
        name, 
        NULL, 
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration
    );

    ImGui::SetWindowPos(position);
    ImGui::SetWindowSize(size);

    rlImGuiImageRenderTextureFit(&m_RenderTexture, true);

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
