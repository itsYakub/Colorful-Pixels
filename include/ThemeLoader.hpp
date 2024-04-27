#pragma once

#include <unordered_map>
#include <string>

#include "imgui.h"

class ThemeLoader {
public:
    bool reloadFont;

private:
    enum CurrentTheme {
        THEME_LIGHT,
        THEME_DARK,
        THEME_COUNT
    } m_CurrentTheme;
    bool m_LoadTheme;

    std::unordered_map<int, std::string> m_LayoutPaths;

    int m_CurrentID;
    bool m_LoadLayout;

public:
    ThemeLoader();

    void Update();
    void LayoutMenu(const char* name, bool draw);
    void ThemeMenu(const char* name, bool draw);

    void LoadLayout(int ID);

    void LoadFont();

    void SetupImGuiStyleDark();
    void SetupImGuiStyleLight();
};