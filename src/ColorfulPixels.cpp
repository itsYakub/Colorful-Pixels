#include "ColorfulPixels.hpp"

#include <memory>

#include "imgui.h"
#include "rlImGui.h"

ColorfulPixels::ColorfulPixels() { }

void ColorfulPixels::Load() {
    m_Viewport = std::make_unique<Viewport>(
        (const Vector2) { 
            192, 
            0 
        }, (const Vector2) { 
            768, 
            512 
        }
    );

    m_Canvas = std::make_unique<Canvas>(m_Viewport.get(), 16);

    rlImGuiSetup(true);
}

void ColorfulPixels::Unload() {
    rlImGuiShutdown();

    m_Viewport->Unload();
    m_Canvas->Unload();
}

void ColorfulPixels::Update() {
    m_Canvas->Update();
}

void ColorfulPixels::Render() {
    m_Viewport->Begin();
    m_Viewport->Clear(DARKBLUE);

        m_Canvas->Render();

    m_Viewport->End();
}

void ColorfulPixels::RenderGUI() {
    ClearBackground(BLACK);

    rlImGuiBegin();

        m_Viewport->ViewportGuiPanel("Panel: Viewport", ImVec2(192.0f, 0.0f), ImVec2(768.0f, 512.0f));

        m_Canvas->ColorGuiPanel("Panel: Colors", ImVec2(0.0f, 0.0f), ImVec2(192.0f, 512.0f));
        m_Canvas->LayersGuiPanel("Panel: Layers", ImVec2(0.0f, 512.0f), ImVec2(1024.0f, 256.0f));
        m_Canvas->ToolsGuiPanel("Panel: Tools", ImVec2(960.0f, 0.0f), ImVec2(64.0f, 512.0f));

    rlImGuiEnd();    
}
