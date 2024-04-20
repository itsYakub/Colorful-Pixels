#include "BrushTool.hpp"

#include "Canvas.hpp"
#include "ColorSystem.hpp"

BrushTool::BrushTool(Canvas* canvas, ColorSystem* colorSystem) : 
    m_Canvas(canvas),
    m_ColorSystem(colorSystem) { }

void BrushTool::OnButtonPress() { }

void BrushTool::OnButtonDown() {
    int ax = m_Canvas->GetMouseCanvasIndex(m_Canvas->MouseWorldPositionScaled()).x;
    int ay = m_Canvas->GetMouseCanvasIndex(m_Canvas->MouseWorldPositionScaled()).y;

    int bx = m_Canvas->GetMouseCanvasIndex(m_Canvas->PreviousFrameMousePositionScaled()).x;
    int by = m_Canvas->GetMouseCanvasIndex(m_Canvas->PreviousFrameMousePositionScaled()).y;

    m_Canvas->DigitalDifferentialAnalyzer(ax, ay, bx, by, m_ColorSystem->GetColor());
}

void BrushTool::OnButtonRelease() { }
