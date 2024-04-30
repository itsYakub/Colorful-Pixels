#include "ColorfulPixels.hpp"

#include <array>
#include <memory>

#include "imgui.h"
#include "imgui_internal.h"
#include "raylib.h"
#include "rlImGui.h"

#include "IconsLucide.h"

ColorfulPixels::ColorfulPixels() :
    m_ThemeLoader(),
    m_IO(),

    m_Viewport(),
    m_ColorSystem(),
    m_ToolSystem(),
    m_Project(m_Viewport, m_ColorSystem, m_ToolSystem),
    
    m_LoadIniFile(true),
    m_IntroPanelImage(),
    m_DrawIntroGuiPanel(true) { }

void ColorfulPixels::Load() {
    LoadImGui();
    m_IntroPanelImage = LoadTexture("../res/images/colorful_pixels_screenshot.png");
    m_Viewport.Load();
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
    m_Project.Unload();

    UnloadTexture(m_IntroPanelImage);

    UnloadImGui();
}

void ColorfulPixels::UnloadImGui() {
    rlImGuiShutdown();
}

void ColorfulPixels::Update() {    
    m_Project.Update();
    m_ThemeLoader.Update();
}

void ColorfulPixels::Render() {
    m_Viewport.Begin();
    m_Viewport.Clear(BLANK);

        m_Project.Render();

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

        if(m_Project.valid) {
            m_Viewport.ViewportGuiPanel("Panel: Viewport", true);
            m_ColorSystem.ColorGuiPanel("Panel: Colors", true);
            m_Project.canvas->layerSystem.LayersGuiPanel("Panel: Layers", true);
            m_ToolSystem.ToolsGuiPanel("Panel: Tools", true, m_Project.tool, m_Project, m_ColorSystem);
        } 

        if(m_DrawIntroGuiPanel) IntroGuiPanel("Panel: Introduction", m_DrawIntroGuiPanel);
        if(m_IO.drawNewProjectGuiPanel) m_IO.NewProject(m_Project);
        if(m_IO.drawExportProjectGuiPanel) m_IO.ExportProject(m_Project, m_Project.canvas->layerSystem);

    EndDockingSpace();
    rlImGuiEnd();    
}

void ColorfulPixels::MenuBarGuiPanel(const char* name, bool draw) {
    if(!draw) {
        return;
    }

    if(ImGui::BeginMenuBar()) {
        if(ImGui::BeginMenu(ICON_LC_FILE_PLUS " File")) {
            if(ImGui::MenuItem(ICON_LC_FILE_PLUS " New...")) {
                TraceLog(LOG_INFO, "Menu option: New");
                m_IO.drawNewProjectGuiPanel = true;
            }

            if(ImGui::MenuItem(ICON_LC_FILE_UP " Load...")) {
                TraceLog(LOG_INFO, "Menu option: Load");
            }

            if(ImGui::MenuItem(ICON_LC_SAVE " Save...")) {
                TraceLog(LOG_INFO, "Menu option: Save");
            }

            if(ImGui::MenuItem(ICON_LC_IMAGE_DOWN " Export...")) {
                if(m_Project.valid) {
                    TraceLog(LOG_INFO, "Menu option: Export");
                    m_IO.drawExportProjectGuiPanel = true;
                }
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

void ColorfulPixels::IntroGuiPanel(const char* name, bool draw) {
    if(!draw) {
        return;
    }

    if(ImGui::Begin(TextFormat("Colorful Pixels %s", COLORFUL_PIXELS_VERSION), &m_DrawIntroGuiPanel, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse)) {
        ImVec2 panelPosition = ImGui::GetWindowPos();
        ImVec2 panelSize = ImGui::GetWindowSize();

        if(ImGui::IsMouseReleased(ImGuiMouseButton_Left) && !ImGui::IsMouseHoveringRect(panelPosition, ImVec2(panelPosition.x + panelSize.x, panelPosition.y + panelSize.y))) {
            m_DrawIntroGuiPanel = false;
            
            ImGui::End();
            return;
        }

        ImGui::SeparatorText("##separator");

        rlImGuiImageRect(
            &m_IntroPanelImage, 
            m_IntroPanelImage.width / 2.0f, 
            m_IntroPanelImage.height / 2.0f, 
            (Rectangle) {
                0.0f,
                0.0f,
                static_cast<float>(m_IntroPanelImage.width),
                static_cast<float>(m_IntroPanelImage.height)
            }
        );

        ImGui::SeparatorText("##separator");

        if(ImGui::Button(ICON_LC_FILE_PLUS " Create")) {
            m_IO.drawNewProjectGuiPanel = true;
            m_DrawIntroGuiPanel = false;
            
            ImGui::End();
            return;
        }

        ImGui::SameLine();

        
        if(ImGui::Button(ICON_LC_FILE_UP " Load")) {
            m_DrawIntroGuiPanel = false;
            
            ImGui::End();
            return;
        }

        ImGui::SameLine();

        
        if(ImGui::Button(ICON_LC_CIRCLE_X " Cancel")) {
            m_DrawIntroGuiPanel = false;
            
            ImGui::End();
            return;
        }

        ImGui::SeparatorText("##separator");

        ImGui::End();
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
