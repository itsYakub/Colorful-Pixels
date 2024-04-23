#include "ToolSystem.hpp"

#include "imgui.h"
#include "raylib.h"

#include "BrushTool.hpp"
#include "EraserTool.hpp"
#include "ColorPickerTool.hpp"
#include "PanTool.hpp"
#include "LineTool.hpp"

ToolSystem::ToolSystem(Canvas* canvas, ColorSystem* colorSystem) :
    m_Canvas(canvas),
    m_ColorSystem(colorSystem),
    m_CurrentTool(std::make_unique<BrushTool>(canvas, colorSystem, &canvas->GetCursor())) { }


void ToolSystem::Update() {
    switch(GetKeyPressed()) {
        case KEY_C:
            SetCurrentTool(new BrushTool(m_Canvas, m_ColorSystem, &m_Canvas->GetCursor()));

            break;

        case KEY_E:
            SetCurrentTool(new EraserTool(m_Canvas, &m_Canvas->GetCursor()));

            break;

        case KEY_V:
            SetCurrentTool(new ColorPickerTool(m_Canvas, m_ColorSystem, this));

            break;

        case KEY_M:
            SetCurrentTool(new PanTool(m_Canvas));

            break;

        case KEY_L:
            SetCurrentTool(new LineTool(m_Canvas, m_ColorSystem, &m_Canvas->GetCursor()));

            break;
    }
}

std::unique_ptr<Tool>& ToolSystem::GetCurrentTool() {
    return m_CurrentTool;
}

void ToolSystem::SetCurrentTool(Tool* newTool) {
    if(m_CurrentTool.get()) {
        m_CurrentTool.release();
    }

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

    ImVec2 size = ImGui::GetWindowSize();

    if(ImGui::Button("##Brush", ImVec2(size.x - 16, size.x - 16))) {
        SetCurrentTool(new BrushTool(m_Canvas, m_ColorSystem, &m_Canvas->GetCursor()));
    } if(ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Tool: Brush\n(Key: C)");
    }

    if(ImGui::Button("##Eraser", ImVec2(size.x - 16, size.x - 16))) {
        SetCurrentTool(new EraserTool(m_Canvas, &m_Canvas->GetCursor()));
    } if(ImGui::IsItemHovered()) {
            ImGui::SetTooltip("Tool: Eraser\n(Key: E)");
    }

    if(ImGui::Button("##Color Picker", ImVec2(size.x - 16, size.x - 16))) {
        SetCurrentTool(new ColorPickerTool(m_Canvas, m_ColorSystem, this));
    } if(ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Tool: Color Picker\n(Key: V)");
    }

    if(ImGui::Button("##Pan Tool", ImVec2(size.x - 16, size.x - 16))) {
        SetCurrentTool(new PanTool(m_Canvas));
    } if(ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Tool: Pan Tool\n(Key: M)");
    }

    if(ImGui::Button("##Line Tool", ImVec2(size.x - 16, size.x - 16))) {
        SetCurrentTool(new LineTool(m_Canvas, m_ColorSystem, &m_Canvas->GetCursor()));
    } if(ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Tool: Line Tool\n(Key: L)");
    }

    ImGui::End(); 
}