#include "LineTool.hpp"

#include "raylib.h"
#include "raymath.h"

#include "Algorithms.hpp"
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

    Algorithms::DigitalDifferentialAnalyzer(ax, ay, bx, by, m_ColorSystem->GetColor(), *m_Canvas->GetLayerSystem().GetLayer());

    TraceLog(
        LOG_INFO, 
        TextFormat(
            "Line drawn for positions: x1.:%i, y1.:%i, x2.:%i, y2.:%i",
            ax, ay,
            bx, by
        )
    );

    m_PointA = Vector2Zero();
    m_PointB = Vector2Zero();

    m_Canvas->ToggleTextureReload();
}
