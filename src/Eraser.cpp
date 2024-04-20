#include "Eraser.hpp"

#include "Canvas.hpp"

Eraser::Eraser(Canvas& canvas) : m_Canvas(canvas) { }

void Eraser::OnButtonPress() { }

void Eraser::OnButtonDown() {
    int ax = m_Canvas.GetMouseCanvasIndex(m_Canvas.MouseWorldPositionScaled()).x;
    int ay = m_Canvas.GetMouseCanvasIndex(m_Canvas.MouseWorldPositionScaled()).y;

    int bx = m_Canvas.GetMouseCanvasIndex(m_Canvas.PreviousFrameMousePositionScaled()).x;
    int by = m_Canvas.GetMouseCanvasIndex(m_Canvas.PreviousFrameMousePositionScaled()).y;

    m_Canvas.DigitalDifferentialAnalyzer(ax, ay, bx, by, BLANK);
}

void Eraser::OnButtonRelease() { }