#include "ColorPickerTool.hpp"

#include "raylib.h"

#include "Canvas.hpp"
#include "ColorSystem.hpp"
#include "ToolSystem.hpp"
#include "BrushTool.hpp"


ColorPickerTool::ColorPickerTool(Canvas* canvas, ColorSystem* colorSystem, ToolSystem* toolSystem) : 
    m_Canvas(canvas),
    m_ColorSystem(colorSystem),
    m_ToolSystem(toolSystem) { }

void ColorPickerTool::OnButtonPress() { 
    int x = m_Canvas->PositionAsCanvasIndex(m_Canvas->PositionInWorldSpace(GetMousePosition())).x;
    int y = m_Canvas->PositionAsCanvasIndex(m_Canvas->PositionInWorldSpace(GetMousePosition())).y;

    if(ColorToInt(m_Canvas->GetLayerSystem().GetLayer()->GetPixelColor(x, y)) != ColorToInt(BLANK)) {
        m_ColorSystem->SetColor(m_Canvas->GetLayerSystem().GetLayer()->GetPixelColor(x, y));
        m_ToolSystem->SetCurrentTool(new BrushTool(m_Canvas, m_ColorSystem, &m_Canvas->GetCursor()));
    }
}

void ColorPickerTool::OnButtonDown() { }

void ColorPickerTool::OnButtonRelease() { }

void ColorPickerTool::Render() { }
