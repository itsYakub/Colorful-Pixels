#include "ColorfulPixels.hpp"

#include <array>
#include <memory>

#include "imgui.h"
#include "imgui_internal.h"
#include "raylib.h"
#include "rlImGui.h"

#include "IconsFontAwesome6.h"
#include "IconsFontAwesome6Brands.h"

ColorfulPixels::ColorfulPixels() :
    m_DrawToolsPanel(true),
    m_DrawLayerPanel(true),
    m_DrawColorPanel(true),
    
    m_LoadIniFile(true) { }

void ColorfulPixels::Load() {
    m_Viewport = std::make_unique<Viewport>();
    m_Canvas = std::make_unique<Canvas>(m_Viewport.get());
    m_ColorSystem = std::make_unique<ColorSystem>();
    m_ToolSystem = std::make_unique<ToolSystem>(m_Canvas.get(), m_ColorSystem.get());
    m_ThemeLoader = std::make_unique<ThemeLoader>();

    LoadImGui();
}

void ColorfulPixels::LoadImGui() {
    rlImGuiSetup(false);

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    m_ThemeLoader->LoadLayout(1);
    m_ThemeLoader->LoadFont();
    m_ThemeLoader->SetupImGuiStyleDark();
}

void ColorfulPixels::Unload() {
    m_Viewport->Unload();
    m_Canvas->Unload();

    UnloadImGui();
}

void ColorfulPixels::UnloadImGui() {
    rlImGuiShutdown();
}

void ColorfulPixels::Update() {    
    // Check if mouse cursor is placed on the viewport...
    if(m_Viewport->IsCursorInViewport()) {        
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

        // ...Check if scroll is pressed
        if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT) || IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
            m_Canvas->Pan();
        }

        // ...Check if scroll accelerates
        if(GetMouseWheelMove() != 0.0f) {
            m_Canvas->Zoom();
        }
    }

    m_Canvas->Update();
    m_ToolSystem->Update();
    m_ThemeLoader->Update();
}

void ColorfulPixels::Render() {
    m_Viewport->Begin();
    m_Viewport->Clear(BLANK);

        m_Canvas->Render();

    m_Viewport->End();
}

void ColorfulPixels::RenderGUI() {
    ClearBackground(BLACK);

    rlImGuiBegin();

    if(m_ThemeLoader->reloadFont) {
        rlImGuiReloadFonts();
        m_ThemeLoader->reloadFont = false;
    }

    BeginDockingSpace("Docking Space");

        m_Viewport->ViewportGuiPanel("Panel: Viewport", true);
        m_ColorSystem->ColorGuiPanel("Panel: Colors", m_DrawColorPanel);
        m_Canvas->GetLayerSystem().LayersGuiPanel("Panel: Layers", m_DrawLayerPanel);
        m_ToolSystem->ToolsGuiPanel("Panel: Tools", m_DrawToolsPanel);

    EndDockingSpace();
    rlImGuiEnd();    
}

void ColorfulPixels::MenuBarGuiPanel(const char* name, bool draw) {
    if(!draw) {
        return;
    }

    if(ImGui::BeginMenuBar()) {
        if(ImGui::BeginMenu(ICON_FA_FILE " File")) {
            if(ImGui::MenuItem(ICON_FA_PLUS " New...")) {
                TraceLog(LOG_INFO, "Menu option: New");
            }

            if(ImGui::MenuItem(ICON_FA_UPLOAD " Load...")) {
                TraceLog(LOG_INFO, "Menu option: Load");
            }

            if(ImGui::MenuItem(ICON_FA_DOWNLOAD " Save...")) {
                TraceLog(LOG_INFO, "Menu option: Save");
            }

            if(ImGui::MenuItem(ICON_FA_FILE_EXPORT " Export...")) {
                TraceLog(LOG_INFO, "Menu option: Export");
            }

            ImGui::EndMenu();
        }
        
        if(ImGui::BeginMenu(ICON_FA_SLIDERS " View")) {
            ImGui::SeparatorText("Options: Canvas");

            ImGui::Checkbox("Canvas: Draw grid", &m_Canvas->drawGrid);
            ImGui::Checkbox("Canvas: Draw frame", &m_Canvas->drawFrame);
            ImGui::Checkbox("Canvas: Draw cursor", &m_Canvas->drawCursor);

            ImGui::NewLine();

            if(m_Canvas->drawGrid) {
                ImGui::SliderFloat("Canvas: Grid thickness", &m_Canvas->gridThickness, 0.0f, 5.0f);
            }

            if(m_Canvas->drawFrame) {
                ImGui::SliderFloat("Canvas: Frame thickness", &m_Canvas->frameThickness, 0.0f, 5.0f);
            }

            if(m_Canvas->drawCursor) {
                ImGui::SliderFloat("Canvas: Cursor thickness", &m_Canvas->cursorThickness, 0.0f, 5.0f);
            }

            ImGui::EndMenu();
        }
        
        if(ImGui::BeginMenu(ICON_FA_WINDOW_MAXIMIZE " Window")) {
            if(ImGui::BeginMenu("Panels")) {
                ImGui::Checkbox("Color", &m_DrawColorPanel);
                ImGui::Checkbox("Layers", &m_DrawLayerPanel);
                ImGui::Checkbox("Tools", &m_DrawToolsPanel);

                ImGui::EndMenu();
            }

            m_ThemeLoader->LayoutMenu("Layout", true);
            m_ThemeLoader->ThemeMenu("Theme", true);

            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu(ICON_FA_USER " Other")) {
            if(ImGui::Button(ICON_FA_GITHUB " Source code...")) {
                OpenURL("https://github.com/itsYakub/Colorful-Pixels");
            }

            if(ImGui::Button(ICON_FA_MUG_SAUCER " Buy me a coffee...")) {
                OpenURL("https://ko-fi.com/yakub");
            }

            if(ImGui::Button(ICON_FA_ITCH_IO " Itch.io page...")) {
                OpenURL("https://itsyakub.itch.io/colorful-pixels");
            }

            ImGui::EndMenu();
        }
        
        ImGui::EndMenuBar();
    }    
}

void ColorfulPixels::BeginDockingSpace(const char* name) {
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

    ImGuiViewport* imGuiViewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImVec2(GetScreenWidth(), GetScreenHeight()), ImGuiCond_Always);
    ImGui::SetNextWindowViewport(imGuiViewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::Begin(name, nullptr, windowFlags);
    ImGui::PopStyleVar(2);

    ImGui::DockSpace(ImGui::GetID(name), ImVec2(0.0f, 0.0f), dockspaceFlags);

    MenuBarGuiPanel("Menu Bar", true);
}

void ColorfulPixels::EndDockingSpace() {
    ImGui::End();
}
