#include "RectTool.hpp"

#include "raylib.h"
#include "raymath.h"

#include "Algorithms.hpp"
#include "Canvas.hpp"
#include "ColorSystem.hpp"
#include "Cursor.hpp"

RectTool::RectTool(Canvas* canvas, ColorSystem* colorSystem) : 
    m_Canvas(canvas),
    m_ColorSystem(colorSystem),
    m_PointA(Vector2Zero()), m_PointB(Vector2Zero()) { }

void RectTool::OnButtonPress() {
    m_PointA = m_Canvas->PositionInWorldSpace(GetMousePosition());
}

void RectTool::OnButtonDown() { 
    m_PointB = m_Canvas->PositionInWorldSpace(GetMousePosition());
}

void RectTool::OnButtonRelease() {
    int ax = m_Canvas->PositionAsCanvasIndex(m_PointA).x;
    int ay = m_Canvas->PositionAsCanvasIndex(m_PointA).y;

    int bx = m_Canvas->PositionAsCanvasIndex(m_PointB).x;
    int by = m_Canvas->PositionAsCanvasIndex(m_PointB).y;

    if(!m_Canvas->CanvasIndexValid({ static_cast<float>(ax), static_cast<float>(ay) }) || !m_Canvas->CanvasIndexValid({ static_cast<float>(bx), static_cast<float>(by) })) {
        return;
    }

    Algorithms::DigitalDifferentialAnalyzer(ax, ay, bx, ay, m_ColorSystem->GetColor(), *m_Canvas->GetLayerSystem().GetLayer());
    Algorithms::DigitalDifferentialAnalyzer(ax, ay, ax, by, m_ColorSystem->GetColor(), *m_Canvas->GetLayerSystem().GetLayer());
    Algorithms::DigitalDifferentialAnalyzer(ax, by, bx, by, m_ColorSystem->GetColor(), *m_Canvas->GetLayerSystem().GetLayer());
    Algorithms::DigitalDifferentialAnalyzer(bx, ay, bx, by, m_ColorSystem->GetColor(), *m_Canvas->GetLayerSystem().GetLayer());

    m_PointA = Vector2Zero();
    m_PointB = Vector2Zero();

    m_Canvas->ToggleTextureReload();
}

void RectTool::Render() { }

