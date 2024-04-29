#pragma once

#include "Tool.hpp"

class Canvas;
class ColorSystem;

class FillTool : public Tool {
private:
    Canvas* m_Canvas;
    ColorSystem* m_ColorSystem;

public:
    FillTool(Canvas* canvas, ColorSystem* colorSystem);
    void OnButtonPress() override;
    void OnButtonDown() override;
    void OnButtonRelease() override;
    void Render() override;
};