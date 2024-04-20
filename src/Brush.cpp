#include "Brush.hpp"

#include "Canvas.hpp"

Brush::Brush(Canvas& canvas) : m_Canvas(canvas) { }

void Brush::OnButtonPress() { }

void Brush::OnButtonDown() {
    int ax = m_Canvas.GetMouseCanvasIndex(m_Canvas.MouseWorldPositionScaled()).x;
    int ay = m_Canvas.GetMouseCanvasIndex(m_Canvas.MouseWorldPositionScaled()).y;

    int bx = m_Canvas.GetMouseCanvasIndex(m_Canvas.PreviousFrameMousePositionScaled()).x;
    int by = m_Canvas.GetMouseCanvasIndex(m_Canvas.PreviousFrameMousePositionScaled()).y;

    m_Canvas.DigitalDifferentialAnalyzer(ax, ay, bx, by, m_Canvas.GetColor());
}

void Brush::OnButtonRelease() { }
