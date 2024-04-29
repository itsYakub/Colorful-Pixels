#pragma once

#include <memory>
#include <vector>

#include "raylib.h"
#include "imgui.h"

#include "Project.hpp"
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

public:
    ToolSystem();

    std::unique_ptr<Tool> SetCurrentTool(ToolList list, Project& project, ColorSystem& colorSystem);

    void ToolsGuiPanel(const char* name, bool draw, std::unique_ptr<Tool>& tool, Project& project, ColorSystem& colorSystem);
};