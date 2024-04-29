#pragma once

#include "Tool.hpp"

class Canvas;
class ColorSystem;

class BrushTool : public Tool {
private:
    Canvas* m_Canvas;
    ColorSystem* m_ColorSystem;

public:
    BrushTool(Canvas* canvas, ColorSystem* colorSystem);
    void OnButtonPress() override;
    void OnButtonDown() override;
    void OnButtonRelease() override;
    void Render() override;
};