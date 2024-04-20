#include "ColorPalette.hpp"

#include "raylib.h"
#include "raymath.h"

ColorPalette::ColorPalette() : m_ColorList() {
    m_ColorList.push_back(LIGHTGRAY);
    m_ColorList.push_back(GRAY);
    m_ColorList.push_back(DARKGRAY);
    m_ColorList.push_back(YELLOW);
    m_ColorList.push_back(GOLD);
    m_ColorList.push_back(ORANGE);
    m_ColorList.push_back(PINK);
    m_ColorList.push_back(RED);
    m_ColorList.push_back(MAROON);
    m_ColorList.push_back(GREEN);
    m_ColorList.push_back(LIME);
    m_ColorList.push_back(DARKGREEN);
    m_ColorList.push_back(SKYBLUE);
    m_ColorList.push_back(BLUE);
    m_ColorList.push_back(DARKBLUE);
    m_ColorList.push_back(PURPLE);
    m_ColorList.push_back(VIOLET);
    m_ColorList.push_back(DARKPURPLE);
    m_ColorList.push_back(BEIGE);
    m_ColorList.push_back(BROWN);
    m_ColorList.push_back(DARKBROWN);
    m_ColorList.push_back(WHITE);
    m_ColorList.push_back(BLACK);
    m_ColorList.push_back(MAGENTA);
    m_ColorList.push_back(RAYWHITE);
}

int ColorPalette::Size() {
    return m_ColorList.size();
}

Color ColorPalette::GetColor(int index) {
    return m_ColorList.at(Clamp(index, 0, m_ColorList.size() - 1));
}
