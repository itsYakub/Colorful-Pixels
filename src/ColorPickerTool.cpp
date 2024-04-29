#include "ColorPickerTool.hpp"

#include "raylib.h"

#include "Canvas.hpp"
#include "Project.hpp"
#include "ColorSystem.hpp"
#include "ToolSystem.hpp"
#include "BrushTool.hpp"


ColorPickerTool::ColorPickerTool(Project* project, ColorSystem* colorSystem) : 
    m_Project(project),
    m_ColorSystem(colorSystem) { }

void ColorPickerTool::OnButtonPress() { 
    int x = m_Project->canvas->PositionAsCanvasIndex(m_Project->canvas->PositionInWorldSpace(GetMousePosition(), m_Project->camera)).x;
    int y = m_Project->canvas->PositionAsCanvasIndex(m_Project->canvas->PositionInWorldSpace(GetMousePosition(), m_Project->camera)).y;

    if(ColorToInt(m_Project->canvas->layerSystem.GetLayer()->GetPixelColor(x, y)) != ColorToInt(BLANK)) {
        m_ColorSystem->SetColor(m_Project->canvas->layerSystem.GetLayer()->GetPixelColor(x, y));
    }
}

void ColorPickerTool::OnButtonDown() { }

void ColorPickerTool::OnButtonRelease() { }

void ColorPickerTool::Render() { }
