#include "ColorfulPixels.hpp"

#include <array>
#include <memory>

#include "ToolSystem.hpp"

#include "imgui.h"
#include "imgui_internal.h"
#include "raylib.h"
#include "rlImGui.h"

#include "IconsLucide.h"

ColorfulPixels::ColorfulPixels() :
    m_Viewport(),
    m_ThemeLoader(),
    m_ColorSystem(),
    m_ToolSystem(),
    
    m_LoadIniFile(true) { }

void ColorfulPixels::Load() {
    LoadImGui();
    m_Viewport.Load();

    m_Canvas = std::make_unique<Canvas>(&m_Viewport);
    m_Tool = m_ToolSystem.SetCurrentTool(ToolSystem::TOOL_BRUSH, *m_Canvas, m_ColorSystem);
}

void ColorfulPixels::LoadImGui() {
    rlImGuiSetup(false);

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    m_ThemeLoader.LoadLayout();
    m_ThemeLoader.LoadFont();
    m_ThemeLoader.SetupImGuiStyleDark();
}

void ColorfulPixels::Unload() {
    m_Viewport.Unload();
    m_Canvas->Unload();

    UnloadImGui();
}

void ColorfulPixels::UnloadImGui() {
    rlImGuiShutdown();
}

void ColorfulPixels::Update() {    
    // Check if mouse cursor is placed on the viewport...
    if(m_Viewport.IsCursorInViewport()) {        
        // ...Check if left mouse button is pressed
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            m_Tool->OnButtonPress();
        }

        // ...Check if left mouse button is down
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            m_Tool->OnButtonDown();
        }

        // ...Check if left mouse button is released
        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            m_Tool->OnButtonRelease();
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
    m_ThemeLoader.Update();
}

void ColorfulPixels::Render() {
    m_Viewport.Begin();
    m_Viewport.Clear(BLANK);

        m_Canvas->Render();
        m_Tool->Render();

    m_Viewport.End();
}

void ColorfulPixels::RenderGUI() {
    ClearBackground(BLACK);

    rlImGuiBegin();

    if(m_ThemeLoader.reloadFont) {
        rlImGuiReloadFonts();
        m_ThemeLoader.reloadFont = false;
    }

    BeginDockingSpace("Docking Space");

        m_Viewport.ViewportGuiPanel("Panel: Viewport", true);
        m_ColorSystem.ColorGuiPanel("Panel: Colors", true);
        m_Canvas->GetLayerSystem().LayersGuiPanel("Panel: Layers", true);
        m_ToolSystem.ToolsGuiPanel("Panel: Tools", true, m_Tool, *m_Canvas, m_ColorSystem);

    EndDockingSpace();
    rlImGuiEnd();    
}

void ColorfulPixels::MenuBarGuiPanel(const char* name, bool draw) {
    if(!draw) {
        return;
    }

    if(ImGui::BeginMenuBar()) {
        if(ImGui::BeginMenu(ICON_LC_FILE " File")) {
            if(ImGui::MenuItem(ICON_LC_PLUS " New...")) {
                TraceLog(LOG_INFO, "Menu option: New");
            }

            if(ImGui::MenuItem(ICON_LC_UPLOAD " Load...")) {
                TraceLog(LOG_INFO, "Menu option: Load");
            }

            if(ImGui::MenuItem(ICON_LC_DOWNLOAD " Save...")) {
                TraceLog(LOG_INFO, "Menu option: Save");
            }

            if(ImGui::MenuItem(ICON_LC_IMAGE_DOWN " Export...")) {
                TraceLog(LOG_INFO, "Menu option: Export");
            }

            ImGui::EndMenu();
        }
        
        if(ImGui::BeginMenu(ICON_LC_APP_WINDOW " Window")) {
            m_ThemeLoader.ThemeMenu("Theme", true);

            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu(ICON_LC_HEART " Other")) {
            if(ImGui::MenuItem(ICON_LC_GITHUB " Source code...")) {
                OpenURL("https://github.com/itsYakub/Colorful-Pixels");
            }

            if(ImGui::MenuItem(ICON_LC_COFFEE " Buy me a coffee...")) {
                OpenURL("https://ko-fi.com/yakub");
            }

            if(ImGui::MenuItem(ICON_LC_WRENCH " Visit the Tool's page")) {
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
