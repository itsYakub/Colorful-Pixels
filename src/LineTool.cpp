#include "LineTool.hpp"

#include "raylib.h"
#include "raymath.h"

#include "Algorithms.hpp"
#include "Project.hpp"
#include "Canvas.hpp"
#include "ColorSystem.hpp"

LineTool::LineTool(int id, Project* project) : 
    m_Project(project),
    m_PointA(Vector2Zero()), m_PointB(Vector2Zero()) { ID = id; }

void LineTool::OnButtonPress() {
    Canvas& canvas = *m_Project->canvas;
    Viewport& viewport = m_Project->viewport;
    
    m_PointA = canvas.PositionInWorldSpace(GetMousePosition(), viewport.GetPosition(), m_Project->camera);
}

void LineTool::OnButtonDown() { 
    Canvas& canvas = *m_Project->canvas;
    Viewport& viewport = m_Project->viewport;
    
    m_PointB = canvas.PositionInWorldSpace(GetMousePosition(), viewport.GetPosition(), m_Project->camera);
}

void LineTool::OnButtonRelease() {
    Canvas& canvas = *m_Project->canvas;
    Viewport& viewport = m_Project->viewport;
    LayerSystem& layerSystem = canvas.layerSystem;
    ColorSystem& colorSystem = m_Project->colorSystem;

    int ax = canvas.PositionAsCanvasIndex(m_PointA).x;
    int ay = canvas.PositionAsCanvasIndex(m_PointA).y;

    int bx = canvas.PositionAsCanvasIndex(m_PointB).x;
    int by = canvas.PositionAsCanvasIndex(m_PointB).y;

    if(!canvas.CanvasIndexValid({ static_cast<float>(ax), static_cast<float>(ay) }) || !canvas.CanvasIndexValid({ static_cast<float>(bx), static_cast<float>(by) })) {
        return;
    }

    Algorithms::DigitalDifferentialAnalyzer(ax, ay, bx, by, colorSystem.GetColor(), layerSystem.GetLayer());

    canvas.ToggleLayerReload();

    m_PointA = Vector2Zero();
    m_PointB = Vector2Zero();
}

void LineTool::Render() {
    Canvas& canvas = *m_Project->canvas;

    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        canvas.DrawCellLines(
            (canvas.PositionAsCanvasCell(m_PointA).x) * canvas.scale,
            (canvas.PositionAsCanvasCell(m_PointA).y) * canvas.scale,
            1.0f,
            RED
        );

        canvas.DrawCellLines(
            (canvas.PositionAsCanvasCell(m_PointB).x) * canvas.scale,
            (canvas.PositionAsCanvasCell(m_PointB).y) * canvas.scale,
            1.0f,
            RED
        );

        DrawLine(
            (canvas.PositionAsCanvasCell(m_PointA).x + canvas.GetCellSize().x / 2.0f / canvas.scale) * canvas.scale, 
            (canvas.PositionAsCanvasCell(m_PointA).y + canvas.GetCellSize().y / 2.0f / canvas.scale) * canvas.scale, 
            (canvas.PositionAsCanvasCell(m_PointB).x + canvas.GetCellSize().x / 2.0f / canvas.scale) * canvas.scale, 
            (canvas.PositionAsCanvasCell(m_PointB).y + canvas.GetCellSize().y / 2.0f / canvas.scale) * canvas.scale, 
            RED
        );
    }
}
