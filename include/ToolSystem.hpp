#pragma once

#include <memory>
#include <vector>

#include "raylib.h"
#include "imgui.h"

#include "Canvas.hpp"
#include "ColorSystem.hpp"
#include "Tool.hpp"

class ToolSystem {
public:
    enum ToolList {
        TOOL_BRUSH = 0,
        TOOL_ERASER,
        TOOL_COLORPICKER,
        TOOL_FILL,
        TOOL_PAN,
        TOOL_LINE,
        TOOL_RECT,
        TOOL_COUNT,
    };

private:
    Canvas* m_Canvas;
    ColorSystem* m_ColorSystem;
    std::unique_ptr<Tool> m_CurrentTool;

public:
    ToolSystem(Canvas* canvas, ColorSystem* colorSystem);

    std::unique_ptr<Tool>& GetCurrentTool();
    std::unique_ptr<Tool> SetCurrentTool(ToolList list);

    void ToolsGuiPanel(const char* name, bool draw);
};