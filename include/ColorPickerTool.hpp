#pragma once

#include "Tool.hpp"

class Canvas;
class ColorSystem;

class ColorPickerTool : public Tool {
private:
    Canvas* m_Canvas;
    ColorSystem* m_ColorSystem;

public:
    ColorPickerTool(Canvas* canvas, ColorSystem* colorSystem);
    void OnButtonPress() override;
    void OnButtonDown() override;
    void OnButtonRelease() override;
    void Render() override;
};