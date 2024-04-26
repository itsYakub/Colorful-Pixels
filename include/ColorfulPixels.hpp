#pragma once

#include <memory>

#include "raylib.h"
#include "imgui.h"

#include "Viewport.hpp"
#include "Canvas.hpp"
#include "ToolSystem.hpp"
#include "ColorSystem.hpp"
#include "ThemeLoader.hpp"

#ifndef COLORFUL_PIXELS_VERSION_RELEASE
    #define COLORFUL_PIXELS_VERSION_RELEASE "Alpha"
#endif

#ifndef COLORFUL_PIXELS_VERSION_MAJOR
    #define COLORFUL_PIXELS_VERSION_MAJOR "1"
#endif

#ifndef COLORFUL_PIXELS_VERSION_MINOR
    #define COLORFUL_PIXELS_VERSION_MINOR "0"
#endif

#ifndef COLORFUL_PIXELS_VERSION
    #define COLORFUL_PIXELS_VERSION "Alpha 1.0"
#endif

class ColorfulPixels {
private:
    std::unique_ptr<Viewport> m_Viewport;
    std::unique_ptr<Canvas> m_Canvas;
    std::unique_ptr<ToolSystem> m_ToolSystem;
    std::unique_ptr<ColorSystem> m_ColorSystem;
    std::unique_ptr<ThemeLoader> m_ThemeLoader;

    bool m_DrawToolsPanel;
    bool m_DrawColorPanel;
    bool m_DrawLayerPanel;

    bool m_LoadIniFile;

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

    void BeginDockingSpace(const char* name);
    void EndDockingSpace();
};