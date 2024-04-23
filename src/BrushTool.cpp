#include "BrushTool.hpp"

#include "raylib.h"

#include "Canvas.hpp"
#include "ColorSystem.hpp"
#include "Cursor.hpp"

BrushTool::BrushTool(Canvas* canvas, ColorSystem* colorSystem, Cursor* cursor) : 
    m_Canvas(canvas),
    m_ColorSystem(colorSystem),
    m_Cursor(cursor) { }

void BrushTool::OnButtonPress() { }

void BrushTool::OnButtonDown() {
    int ax = m_Canvas->PositionAsCanvasIndex(m_Canvas->PositionInWorldSpace(GetMousePosition())).x;
    int ay = m_Canvas->PositionAsCanvasIndex(m_Canvas->PositionInWorldSpace(GetMousePosition())).y;

    int bx = m_Canvas->PositionAsCanvasIndex(m_Canvas->PositionInWorldSpace(m_Cursor->GetPreviousFramePosition())).x;
    int by = m_Canvas->PositionAsCanvasIndex(m_Canvas->PositionInWorldSpace(m_Cursor->GetPreviousFramePosition())).y;

    if(!m_Canvas->CanvasIndexValid({ static_cast<float>(ax), static_cast<float>(ay) }) || !m_Canvas->CanvasIndexValid({ static_cast<float>(bx), static_cast<float>(by) })) {
        return;
    }

    m_Canvas->DigitalDifferentialAnalyzer(ax, ay, bx, by, m_ColorSystem->GetColor());
}

void BrushTool::OnButtonRelease() { }
