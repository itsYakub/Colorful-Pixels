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

    Vector2 m_PointA;
    Vector2 m_PointB;

public:
    RectTool(Canvas* canvas, ColorSystem* colorSystem);
    void OnButtonPress() override;
    void OnButtonDown() override;
    void OnButtonRelease() override;
    void Render() override;
};