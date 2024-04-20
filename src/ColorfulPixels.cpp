#include "ColorfulPixels.hpp"

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
    m_ColorSystem = std::make_unique<ColorSystem>();
    m_ToolSystem = std::make_unique<ToolSystem>(m_Canvas.get(), m_ColorSystem.get());

    rlImGuiSetup(true);
}

void ColorfulPixels::Unload() {
    rlImGuiShutdown();

    m_Viewport->Unload();
    m_Canvas->Unload();
}

void ColorfulPixels::Update() {
    // Check if mouse cursor is placed on the viewport...
    if(m_Canvas->MouseViewportHover()) {
        // ...Check if mouse cursor is placed on the canvas...
        if(m_Canvas->IsMouseCanvasIndexValid(m_Canvas->MouseWorldPositionScaled()) && m_Canvas->IsMouseCanvasIndexValid(m_Canvas->PreviousFrameMousePositionScaled())) {
            // ...Check if left mouse button is pressed
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                m_ToolSystem->GetCurrentTool()->OnButtonPress();
            }

            // ...Check if left mouse button is down
            if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                m_ToolSystem->GetCurrentTool()->OnButtonDown();
            }

            // ...Check if left mouse button is released
            if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                m_ToolSystem->GetCurrentTool()->OnButtonRelease();
            }
        }

        // ...Check if scroll is pressed
        if(IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
            m_Canvas->Pan();
        }

        // ...Check if scroll accelerates
        if(GetMouseWheelMove() != 0.0f) {
            m_Canvas->Zoom();
        }
    }

    m_Canvas->Update();
    m_ToolSystem->Update();
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

        m_ColorSystem->ColorGuiPanel("Panel: Colors", ImVec2(0.0f, 0.0f), ImVec2(192.0f, 512.0f));
        m_Canvas->GetLayerSystem().LayersGuiPanel("Panel: Layers", ImVec2(0.0f, 512.0f), ImVec2(1024.0f, 256.0f));
        m_ToolSystem->ToolsGuiPanel("Panel: Tools", ImVec2(960.0f, 0.0f), ImVec2(64.0f, 512.0f));

    rlImGuiEnd();    
}
