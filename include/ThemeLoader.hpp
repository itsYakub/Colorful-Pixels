#pragma once

#include <unordered_map>
#include <string>

#include "imgui.h"

class ThemeLoader {
private:
    enum CurrentTheme {
        THEME_LIGHT,
        THEME_DARK,
        THEME_COUNT
    } m_CurrentTheme;

    std::unordered_map<int, std::string> m_LayoutPaths;

    int m_CurrentID;
    bool m_LoadLayout;

    ImFont m_Font;

public:
    ThemeLoader();

    void Update();
    void LayoutMenu(const char* name, bool draw);
    void ThemeMenu(const char* name, bool draw);

    void LoadLayout(int ID);

    void SetupImGuiStyleDark();
    void SetupImGuiStyleLight();
};