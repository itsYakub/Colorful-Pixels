#pragma once

#include "Tool.hpp"

class Canvas;
class ColorSystem;
class Cursor;

class BrushTool : public Tool {
private:
    Canvas* m_Canvas;
    ColorSystem* m_ColorSystem;
    Cursor* m_Cursor;

public:
    BrushTool(Canvas* canvas, ColorSystem* colorSystem, Cursor* cursor);
    void OnButtonPress() override;
    void OnButtonDown() override;
    void OnButtonRelease() override;
    void Render() override;
};