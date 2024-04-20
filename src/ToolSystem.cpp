#include "ToolSystem.hpp"

#include "BrushTool.hpp"
#include "EraserTool.hpp"
#include "ColorPickerTool.hpp"

ToolSystem::ToolSystem(Canvas* canvas, ColorSystem* colorSystem) :
    m_Canvas(canvas),
    m_ColorSystem(colorSystem),
    m_CurrentTool(std::make_unique<BrushTool>(canvas, colorSystem)) { }


void ToolSystem::Update() {
    switch(GetKeyPressed()) {
        case KEY_C:
            SetCurrentTool(new BrushTool(m_Canvas, m_ColorSystem));

            break;

        case KEY_E:
            SetCurrentTool(new EraserTool(m_Canvas));

            break;

        case KEY_V:
            SetCurrentTool(new ColorPickerTool(m_Canvas, m_ColorSystem, this));

            break;
    }
}

std::unique_ptr<Tool>& ToolSystem::GetCurrentTool() {
    return m_CurrentTool;
}

void ToolSystem::SetCurrentTool(Tool* newTool) {
    m_CurrentTool.release();
    m_CurrentTool = std::unique_ptr<Tool>(newTool);

    TraceLog(LOG_INFO, "Current tool: %s", typeid(*newTool).name());
}

void ToolSystem::ToolsGuiPanel(const char* name, ImVec2 position, ImVec2 size) {
    ImGui::Begin(
        name, 
        NULL, 
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration
    );

    ImGui::SetWindowPos(position);
    ImGui::SetWindowSize(size);

    if(ImGui::Button("##Brush", ImVec2(size.x - 16, size.x - 16))) {
        SetCurrentTool(new BrushTool(m_Canvas, m_ColorSystem));
    } if(ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Tool: Brush\n(Key: C)");
    }

    if(ImGui::Button("##Eraser", ImVec2(size.x - 16, size.x - 16))) {
        SetCurrentTool(new EraserTool(m_Canvas));
    } if(ImGui::IsItemHovered()) {
            ImGui::SetTooltip("Tool: Eraser\n(Key: E)");
    }

    if(ImGui::Button("##Color Picker", ImVec2(size.x - 16, size.x - 16))) {
        SetCurrentTool(new ColorPickerTool(m_Canvas, m_ColorSystem, this));
    } if(ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Tool: Color Picker\n(Key: V)");
    }

    ImGui::End(); 
}