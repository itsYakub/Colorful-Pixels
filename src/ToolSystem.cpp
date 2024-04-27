#include "ToolSystem.hpp"

#include "imgui.h"
#include "raylib.h"

#include "IconsFontAwesome6.h"

#include "BrushTool.hpp"
#include "EraserTool.hpp"
#include "ColorPickerTool.hpp"
#include "PanTool.hpp"
#include "LineTool.hpp"
#include "FillTool.hpp"
#include "RectTool.hpp"

ToolSystem::ToolSystem(Canvas* canvas, ColorSystem* colorSystem) :
    m_Canvas(canvas),
    m_ColorSystem(colorSystem),
    m_ToolList(TOOL_BRUSH) {
        SetCurrentTool(new BrushTool(m_Canvas, m_ColorSystem, &m_Canvas->GetCursor()));
    }


void ToolSystem::Update() {
    switch(GetKeyPressed()) {
        case KEY_C:
            SetCurrentTool(new BrushTool(m_Canvas, m_ColorSystem, &m_Canvas->GetCursor()));
            m_ToolList = TOOL_BRUSH;

            break;

        case KEY_E:
            SetCurrentTool(new EraserTool(m_Canvas, &m_Canvas->GetCursor()));
            m_ToolList = TOOL_ERASER;

            break;

        case KEY_V:
            SetCurrentTool(new ColorPickerTool(m_Canvas, m_ColorSystem, this));
            m_ToolList = TOOL_COLORPICKER;

            break;

        case KEY_F:
            SetCurrentTool(new FillTool(m_Canvas, m_ColorSystem, &m_Canvas->GetCursor()));
            m_ToolList = TOOL_FILL;

            break;

        case KEY_M:
            SetCurrentTool(new PanTool(m_Canvas));
            m_ToolList = TOOL_PAN;

            break;

        case KEY_L:
            SetCurrentTool(new LineTool(m_Canvas, m_ColorSystem, &m_Canvas->GetCursor()));
            m_ToolList = TOOL_LINE;

            break;

        case KEY_R:
            SetCurrentTool(new RectTool(m_Canvas, m_ColorSystem, &m_Canvas->GetCursor()));
            m_ToolList = TOOL_RECT;

            break;
    }
}

std::unique_ptr<Tool>& ToolSystem::GetCurrentTool() {
    return m_CurrentTool;
}

void ToolSystem::SetCurrentTool(Tool* newTool) {
    m_CurrentTool = std::unique_ptr<Tool>(newTool);

    TraceLog(LOG_INFO, "Current tool: %s", typeid(*newTool).name());
}

void ToolSystem::ToolsGuiPanel(const char* name, bool draw) {
    if(!draw) {
        return;
    }

    ImGui::Begin(
        name, 
        nullptr
    );

    ImVec2 size(40.0f, 40.0f);

    ImGui::SeparatorText("##text");

    if(ImGui::Button(ICON_FA_BRUSH, size)) {
        SetCurrentTool(new BrushTool(m_Canvas, m_ColorSystem, &m_Canvas->GetCursor()));
        m_ToolList = TOOL_BRUSH;
    } if(ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Tool: Brush\n(Key: C)");
    }

    if(ImGui::Button(ICON_FA_ERASER, size)) {
        SetCurrentTool(new EraserTool(m_Canvas, &m_Canvas->GetCursor()));
        m_ToolList = TOOL_ERASER;
    } if(ImGui::IsItemHovered()) {
            ImGui::SetTooltip("Tool: Eraser\n(Key: E)");
    }

    ImGui::SeparatorText("##text");

    if(ImGui::Button(ICON_FA_EYE_DROPPER, size)) {
        SetCurrentTool(new ColorPickerTool(m_Canvas, m_ColorSystem, this));
        m_ToolList = TOOL_COLORPICKER;
    } if(ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Tool: Color Picker\n(Key: V)");
    }

    if(ImGui::Button(ICON_FA_FILL, size)) {
        SetCurrentTool(new FillTool(m_Canvas, m_ColorSystem, &m_Canvas->GetCursor()));
        m_ToolList = TOOL_FILL;
    } if(ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Tool: Fill Tool\n(Key: F)");
    }

    ImGui::SeparatorText("##text");

    if(ImGui::Button(ICON_FA_ARROWS_UP_DOWN_LEFT_RIGHT, size)) {
        SetCurrentTool(new PanTool(m_Canvas));
        m_ToolList = TOOL_PAN;
    } if(ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Tool: Pan Tool\n(Key: M)");
    }

    ImGui::SeparatorText("##text");

    if(ImGui::Button("?", size)) {
        SetCurrentTool(new LineTool(m_Canvas, m_ColorSystem, &m_Canvas->GetCursor()));
        m_ToolList = TOOL_LINE;
    } if(ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Tool: Line Tool\n(Key: L)");
    }

    if(ImGui::Button("?", size)) {
        SetCurrentTool(new RectTool(m_Canvas, m_ColorSystem, &m_Canvas->GetCursor()));
        m_ToolList = TOOL_RECT;
    } if(ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Tool: Rect Tool\n(Key: R)");
    }

    ImGui::End(); 
}