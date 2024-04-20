#pragma once

#include <memory>

#include "imgui.h"

#include "Canvas.hpp"
#include "ColorSystem.hpp"
#include "Tool.hpp"

class ToolSystem {
private:
    Canvas* m_Canvas;
    ColorSystem* m_ColorSystem;
    std::unique_ptr<Tool> m_CurrentTool;

public:
    ToolSystem(Canvas* canvas, ColorSystem* colorSystem);

    void Update();

    std::unique_ptr<Tool>& GetCurrentTool();
    void SetCurrentTool(Tool* newTool);

    void ToolsGuiPanel(const char* name, ImVec2 position, ImVec2 size);
};