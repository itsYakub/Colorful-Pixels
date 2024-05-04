#include "ToolSystem.hpp"

#include <memory>
#include <typeinfo>

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

ToolSystem::ToolSystem() { }

std::unique_ptr<Tool> ToolSystem::SetCurrentTool(ToolList list, Project& project) {
    Tool* result;

    switch(list) {
        case TOOL_BRUSH: result = new BrushTool(static_cast<int>(TOOL_BRUSH), &project); break;
        case TOOL_ERASER: result = new EraserTool(static_cast<int>(TOOL_ERASER), &project); break;
        case TOOL_COLORPICKER: result = new ColorPickerTool(static_cast<int>(TOOL_COLORPICKER), &project); break;
        case TOOL_FILL: result = new FillTool(static_cast<int>(TOOL_FILL), &project); break;
        case TOOL_PAN: result = new PanTool(static_cast<int>(TOOL_PAN), &project); break;
        case TOOL_LINE: result = new LineTool(static_cast<int>(TOOL_LINE), &project); break;
        case TOOL_RECT: result = new RectTool(static_cast<int>(TOOL_RECT), &project); break;
        default: result = new BrushTool(static_cast<int>(TOOL_BRUSH), &project); break;
    }

    TraceLog(LOG_INFO, TextFormat("Current tool: %s", typeid(*result).name()));
    return std::unique_ptr<Tool>(result);
}

void ToolSystem::ToolsGuiPanel(const char* name, bool draw, std::unique_ptr<Tool>& tool, Project& project) {
    if(!draw) {
        return;
    }

    if(ImGui::Begin(name, nullptr)) {
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

        ImVec2 size(40.0f, 40.0f);

        ImGui::SeparatorText("##text");

        for(int i = 0; i < ToolList::TOOL_COUNT; i++) {
            if(ImGui::Button(ICONS[i], size)) {
                tool = SetCurrentTool(static_cast<ToolList>(i), project);
            } if(ImGui::IsItemHovered()) {
                ImGui::SetTooltip("%s", TOOLTIPS[i]);
            }
        }

        ImGui::End();
    }
}