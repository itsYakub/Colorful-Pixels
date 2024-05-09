#include "EraserTool.hpp"

#include "raylib.h"

#include "Algorithms.hpp"
#include "Project.hpp"
#include "Canvas.hpp"

EraserTool::EraserTool(int id, Project* project) : 
    m_Project(project) { ID = id; }

void EraserTool::OnButtonPress() { }

void EraserTool::OnButtonDown() {
    Canvas& canvas = *m_Project->canvas;
    Viewport& viewport = m_Project->viewport;
    LayerSystem& layerSystem = canvas.layerSystem;

    int ax = canvas.PositionAsCanvasIndex(canvas.PositionInWorldSpace(GetMousePosition(), viewport.GetPosition(), m_Project->camera)).x;
    int ay = canvas.PositionAsCanvasIndex(canvas.PositionInWorldSpace(GetMousePosition(), viewport.GetPosition(), m_Project->camera)).y;

    int bx = canvas.PositionAsCanvasIndex(canvas.PositionInWorldSpace(canvas.cursor.GetPreviousFramePosition(), viewport.GetPosition(), m_Project->camera)).x;
    int by = canvas.PositionAsCanvasIndex(canvas.PositionInWorldSpace(canvas.cursor.GetPreviousFramePosition(), viewport.GetPosition(), m_Project->camera)).y;

    if(!canvas.CanvasIndexValid({ static_cast<float>(ax), static_cast<float>(ay) }) || !canvas.CanvasIndexValid({ static_cast<float>(bx), static_cast<float>(by) })) {
        return;
    }

    Algorithms::DigitalDifferentialAnalyzer(ax, ay, bx, by, BLANK, layerSystem.GetLayer());

    canvas.ToggleLayerReload();
}

void EraserTool::OnButtonRelease() { }

void EraserTool::Render() { }

