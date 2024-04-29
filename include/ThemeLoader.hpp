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

public:
    ThemeLoader();

    void Update();
    void ThemeMenu(const char* name, bool draw);

    void LoadLayout();
    void LoadFont();

    void SetupImGuiStyleDark();
    void SetupImGuiStyleLight();
};