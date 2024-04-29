#include "ColorPickerTool.hpp"

#include "raylib.h"

#include "Canvas.hpp"
#include "ColorSystem.hpp"
#include "ToolSystem.hpp"
#include "BrushTool.hpp"


ColorPickerTool::ColorPickerTool(Canvas* canvas, ColorSystem* colorSystem) : 
    m_Canvas(canvas),
    m_ColorSystem(colorSystem) { }

void ColorPickerTool::OnButtonPress() { 
    int x = m_Canvas->PositionAsCanvasIndex(m_Canvas->PositionInWorldSpace(GetMousePosition())).x;
    int y = m_Canvas->PositionAsCanvasIndex(m_Canvas->PositionInWorldSpace(GetMousePosition())).y;

    if(ColorToInt(m_Canvas->GetLayerSystem().GetLayer()->GetPixelColor(x, y)) != ColorToInt(BLANK)) {
        m_ColorSystem->SetColor(m_Canvas->GetLayerSystem().GetLayer()->GetPixelColor(x, y));
    }
}

void ColorPickerTool::OnButtonDown() { }

void ColorPickerTool::OnButtonRelease() { }

void ColorPickerTool::Render() { }
