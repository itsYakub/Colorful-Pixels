#include "FillTool.hpp"

#include "raylib.h"

#include "Algorithms.hpp"
#include "Canvas.hpp"
#include "ColorSystem.hpp"
#include "Cursor.hpp"

FillTool::FillTool(Canvas* canvas, ColorSystem* colorSystem, Cursor* cursor) : 
    m_Canvas(canvas),
    m_ColorSystem(colorSystem),
    m_Cursor(cursor) { }

void FillTool::OnButtonPress() {
    int x = m_Canvas->PositionAsCanvasIndex(m_Canvas->PositionInWorldSpace(GetMousePosition())).x;
    int y = m_Canvas->PositionAsCanvasIndex(m_Canvas->PositionInWorldSpace(GetMousePosition())).y;

    if(!m_Canvas->CanvasIndexValid({ static_cast<float>(x), static_cast<float>(y) })) {
        return;
    }

    Algorithms::FloodFillAlgorithm(x, y, m_Canvas->GetLayerSystem().GetLayer()->GetPixelColor(x, y), m_ColorSystem->GetColor(), *m_Canvas->GetLayerSystem().GetLayer());

    m_Canvas->ToggleTextureReload();
}

void FillTool::OnButtonDown() { }

void FillTool::OnButtonRelease() { }

void FillTool::Render() { }
