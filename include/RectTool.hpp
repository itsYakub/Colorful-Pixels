#pragma once

#include "raylib.h"

#include "Tool.hpp"

class Canvas;
class ColorSystem;
class Cursor;

class RectTool : public Tool {
private:
    Canvas* m_Canvas;
    ColorSystem* m_ColorSystem;
    Cursor* m_Cursor;

    Vector2 m_PointA;
    Vector2 m_PointB;

public:
    RectTool(Canvas* canvas, ColorSystem* colorSystem, Cursor* cursor);
    void OnButtonPress() override;
    void OnButtonDown() override;
    void OnButtonRelease() override;
};