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
#include "utils.h"
#include <memory>

ToolSystem::ToolSystem() { }

std::unique_ptr<Tool> ToolSystem::SetCurrentTool(ToolList list, Canvas& canvas, ColorSystem& colorSystem) {
    switch(list) {
        case TOOL_BRUSH: return std::make_unique<BrushTool>(&canvas, &colorSystem);
        case TOOL_ERASER: return std::make_unique<EraserTool>(&canvas);
        case TOOL_COLORPICKER: return std::make_unique<ColorPickerTool>(&canvas, &colorSystem);
        case TOOL_FILL: return std::make_unique<FillTool>(&canvas, &colorSystem);
        case TOOL_PAN: return std::make_unique<PanTool>(&canvas);
        case TOOL_LINE: return std::make_unique<LineTool>(&canvas, &colorSystem);
        case TOOL_RECT: return std::make_unique<RectTool>(&canvas, &colorSystem);
        default: return std::make_unique<BrushTool>(&canvas, &colorSystem);
    }
}

void ToolSystem::ToolsGuiPanel(const char* name, bool draw, std::unique_ptr<Tool>& tool, Canvas& canvas, ColorSystem& colorSystem) {
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
                tool = SetCurrentTool(static_cast<ToolList>(i), canvas, colorSystem);
            } if(ImGui::IsItemHovered()) {
                ImGui::SetTooltip("%s", TOOLTIPS[i]);
            }
        }

        ImGui::End();
    }
}