#include "EraserTool.hpp"

#include "Canvas.hpp"
#include "Cursor.hpp"

EraserTool::EraserTool(Canvas* canvas, Cursor* cursor) : 
    m_Canvas(canvas),
    m_Cursor(cursor) { }

void EraserTool::OnButtonPress() { }

void EraserTool::OnButtonDown() {
    int ax = m_Canvas->PositionAsCanvasIndex(m_Canvas->PositionInWorldSpace(GetMousePosition())).x;
    int ay = m_Canvas->PositionAsCanvasIndex(m_Canvas->PositionInWorldSpace(GetMousePosition())).y;

    int bx = m_Canvas->PositionAsCanvasIndex(m_Canvas->PositionInWorldSpace(m_Cursor->GetPreviousFramePosition())).x;
    int by = m_Canvas->PositionAsCanvasIndex(m_Canvas->PositionInWorldSpace(m_Cursor->GetPreviousFramePosition())).y;

    if(!m_Canvas->CanvasIndexValid({ static_cast<float>(ax), static_cast<float>(ay) }) || !m_Canvas->CanvasIndexValid({ static_cast<float>(bx), static_cast<float>(by) })) {
        return;
    }

    m_Canvas->DigitalDifferentialAnalyzer(ax, ay, bx, by, BLANK);
}

void EraserTool::OnButtonRelease() { }
