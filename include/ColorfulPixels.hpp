#pragma once

#include <memory>

#include "raylib.h"
#include "imgui.h"

#include "Viewport.hpp"
#include "ToolSystem.hpp"
#include "ColorSystem.hpp"
#include "ThemeLoader.hpp"
#include "Project.hpp"
#include "IO.hpp"

#ifndef COLORFUL_PIXELS_VERSION_RELEASE
    #define COLORFUL_PIXELS_VERSION_RELEASE "Alpha"
#endif

#ifndef COLORFUL_PIXELS_VERSION_MAJOR
    #define COLORFUL_PIXELS_VERSION_MAJOR 1
#endif

#ifndef COLORFUL_PIXELS_VERSION_MINOR
    #define COLORFUL_PIXELS_VERSION_MINOR 0
#endif

#ifndef COLORFUL_PIXELS_VERSION
    #define COLORFUL_PIXELS_VERSION "Alpha 1.0"
#endif

class ColorfulPixels {
private:
    std::unique_ptr<Project> m_Project;

    ThemeLoader m_ThemeLoader;
    IO m_IO;

    Viewport m_Viewport;
    ColorSystem m_ColorSystem;
    ToolSystem m_ToolSystem;

    bool m_LoadIniFile;

    Texture2D m_IntroPanelImage;

public:
    ColorfulPixels();

    void Load();
    void LoadImGui();

    void Unload();
    void UnloadImGui();

    void Update();
    void Render();
    void RenderGUI();

    void MenuBarGuiPanel(const char* name, bool draw);
    void IntroGuiPanel(const char* name, bool& draw);

    void BeginDockingSpace(const char* name);
    void EndDockingSpace();
};