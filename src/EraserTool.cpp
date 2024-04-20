#include "EraserTool.hpp"

#include "Canvas.hpp"

EraserTool::EraserTool(Canvas* canvas) : m_Canvas(canvas) { }

void EraserTool::OnButtonPress() { }

void EraserTool::OnButtonDown() {
    int ax = m_Canvas->GetMouseCanvasIndex(m_Canvas->MouseWorldPositionScaled()).x;
    int ay = m_Canvas->GetMouseCanvasIndex(m_Canvas->MouseWorldPositionScaled()).y;

    int bx = m_Canvas->GetMouseCanvasIndex(m_Canvas->PreviousFrameMousePositionScaled()).x;
    int by = m_Canvas->GetMouseCanvasIndex(m_Canvas->PreviousFrameMousePositionScaled()).y;

    m_Canvas->DigitalDifferentialAnalyzer(ax, ay, bx, by, BLANK);
}

void EraserTool::OnButtonRelease() { }
