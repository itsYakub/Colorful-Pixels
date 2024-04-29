#include "ToolSystem.hpp"

#include "imgui.h"
#include "raylib.h"

#include "IconsLucide.h"

#include "BrushTool.hpp"
#include "EraserTool.hpp"
#include "ColorPickerTool.hpp"
#include "PanTool.hpp"
#include "LineTool.hpp"
#include "FillTool.hpp"
#include "RectTool.hpp"
#include <memory>

ToolSystem::ToolSystem(Canvas* canvas, ColorSystem* colorSystem) :
    m_Canvas(canvas),
    m_ColorSystem(colorSystem) {
        m_CurrentTool = SetCurrentTool(TOOL_BRUSH);
    }


std::unique_ptr<Tool>& ToolSystem::GetCurrentTool() {
    return m_CurrentTool;
}

std::unique_ptr<Tool> ToolSystem::SetCurrentTool(ToolList list) {
    switch(list) {
        case TOOL_BRUSH: return std::make_unique<BrushTool>(m_Canvas, m_ColorSystem);
        case TOOL_ERASER: return std::make_unique<EraserTool>(m_Canvas);
        case TOOL_COLORPICKER: return std::make_unique<ColorPickerTool>(m_Canvas, m_ColorSystem, this);
        case TOOL_FILL: return std::make_unique<FillTool>(m_Canvas, m_ColorSystem);
        case TOOL_PAN: return std::make_unique<PanTool>(m_Canvas);
        case TOOL_LINE: return std::make_unique<LineTool>(m_Canvas, m_ColorSystem);
        case TOOL_RECT: return std::make_unique<RectTool>(m_Canvas, m_ColorSystem);
        default: return std::make_unique<BrushTool>(m_Canvas, m_ColorSystem);
    }
}

void ToolSystem::ToolsGuiPanel(const char* name, bool draw) {
    if(!draw) {
        return;
    }

    const char* ICONS[] {
        ICON_LC_BRUSH,
        ICON_LC_ERASER,
        ICON_LC_PIPETTE,
        ICON_LC_PAINT_BUCKET,
        ICON_LC_MOVE,
        ICON_LC_PENCIL_LINE,
        ICON_LC_SQUARE
    };

    const char* TOOLTIPS[] {
        "Tool: Brush",
        "Tool: Eraser",
        "Tool: Color Picker",
        "Tool: Fill",
        "Tool: Pan",
        "Tool: Line",
        "Tool: Rect"
    };

    ImGui::Begin(
        name, 
        nullptr
    );

    ImVec2 size(40.0f, 40.0f);

    ImGui::SeparatorText("##text");

    for(int i = 0; i < ToolList::TOOL_COUNT; i++) {
        if(ImGui::Button(ICONS[i], size)) {
            m_CurrentTool = SetCurrentTool(static_cast<ToolList>(i));
        } if(ImGui::IsItemHovered()) {
            ImGui::SetTooltip(TOOLTIPS[i]);
        }
    }
    
    ImGui::End(); 
}