#pragma once

#include "Tool.hpp"

class Canvas;
class ColorSystem;
class ToolSystem;

class ColorPickerTool : public Tool {
private:
    Canvas* m_Canvas;
    ColorSystem* m_ColorSystem;
    ToolSystem* m_ToolSystem;

public:
    ColorPickerTool(Canvas* canvas, ColorSystem* colorSystem, ToolSystem* toolSystem);
    void OnButtonPress() override;
    void OnButtonDown() override;
    void OnButtonRelease() override;
};