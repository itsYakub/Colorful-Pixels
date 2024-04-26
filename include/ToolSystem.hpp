#pragma once

#include <memory>

#include "raylib.h"
#include "imgui.h"

#include "Canvas.hpp"
#include "ColorSystem.hpp"
#include "Tool.hpp"

class ToolSystem {
private:
    Canvas* m_Canvas;
    ColorSystem* m_ColorSystem;
    std::unique_ptr<Tool> m_CurrentTool;

    enum ToolList {
        TOOL_BRUSH = 0,
        TOOL_ERASER,
        TOOL_COLORPICKER,
        TOOL_FILL,
        TOOL_PAN,
        TOOL_LINE,
        TOOL_RECT,
        TOOL_COUNT,
    } m_ToolList;

public:
    ToolSystem(Canvas* canvas, ColorSystem* colorSystem);

    void Update();

    std::unique_ptr<Tool>& GetCurrentTool();
    void SetCurrentTool(Tool* newTool);

    void ToolsGuiPanel(const char* name, bool draw);
};