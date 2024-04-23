#include "LineTool.hpp"

#include "raylib.h"
#include "raymath.h"

#include "Canvas.hpp"
#include "ColorSystem.hpp"
#include "Cursor.hpp"

LineTool::LineTool(Canvas* canvas, ColorSystem* colorSystem, Cursor* cursor) : 
    m_Canvas(canvas),
    m_ColorSystem(colorSystem),
    m_Cursor(cursor),
    m_PointA(Vector2Zero()), m_PointB(Vector2Zero()) { }

void LineTool::OnButtonPress() {
    m_PointA = m_Canvas->PositionAsCanvasIndex(m_Canvas->PositionInWorldSpace(GetMousePosition()));
}

void LineTool::OnButtonDown() { }

void LineTool::OnButtonRelease() {
    m_PointB = m_Canvas->PositionAsCanvasIndex(m_Canvas->PositionInWorldSpace(GetMousePosition()));

    int ax = m_PointA.x;
    int ay = m_PointA.y;

    int bx = m_PointB.x;
    int by = m_PointB.y;


    if(!m_Canvas->CanvasIndexValid({ static_cast<float>(ax), static_cast<float>(ay) }) || !m_Canvas->CanvasIndexValid({ static_cast<float>(bx), static_cast<float>(by) })) {
        return;
    }

    m_Canvas->DigitalDifferentialAnalyzer(ax, ay, bx, by, m_ColorSystem->GetColor());

    m_PointA = Vector2Zero();
    m_PointB = Vector2Zero();
}
