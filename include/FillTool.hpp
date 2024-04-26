#pragma once

#include "Tool.hpp"

class Canvas;
class ColorSystem;
class Cursor;

class FillTool : public Tool {
private:
    Canvas* m_Canvas;
    ColorSystem* m_ColorSystem;
    Cursor* m_Cursor;

public:
    FillTool(Canvas* canvas, ColorSystem* colorSystem, Cursor* cursor);
    void OnButtonPress() override;
    void OnButtonDown() override;
    void OnButtonRelease() override;
};