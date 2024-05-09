#include "BrushTool.hpp"

#include "raylib.h"

#include "LayerSystem.hpp"
#include "Viewport.hpp"
#include "Algorithms.hpp"
#include "Canvas.hpp"
#include "Project.hpp"
#include "ColorSystem.hpp"

BrushTool::BrushTool(int id, Project* project) : 
    m_Project(project) { ID = id; }

void BrushTool::OnButtonPress() { }

void BrushTool::OnButtonDown() {
    Canvas& canvas = *m_Project->canvas;
    Viewport& viewport = m_Project->viewport;
    LayerSystem& layerSystem = canvas.layerSystem;
    ColorSystem& colorSystem = m_Project->colorSystem;

    if(canvas.layerSystem.GetLayer().layerLocked) {
        return;
    }

    int ax = canvas.PositionAsCanvasIndex(canvas.PositionInWorldSpace(GetMousePosition(), viewport.GetPosition(), m_Project->camera)).x;
    int ay = canvas.PositionAsCanvasIndex(canvas.PositionInWorldSpace(GetMousePosition(), viewport.GetPosition(), m_Project->camera)).y;

    int bx = canvas.PositionAsCanvasIndex(canvas.PositionInWorldSpace(canvas.cursor.GetPreviousFramePosition(), viewport.GetPosition(), m_Project->camera)).x;
    int by = canvas.PositionAsCanvasIndex(canvas.PositionInWorldSpace(canvas.cursor.GetPreviousFramePosition(), viewport.GetPosition(), m_Project->camera)).y;

    if(!canvas.CanvasIndexValid({ static_cast<float>(ax), static_cast<float>(ay) }) || !canvas.CanvasIndexValid({ static_cast<float>(bx), static_cast<float>(by) })) {
        return;
    }

    Algorithms::DigitalDifferentialAnalyzer(ax, ay, bx, by, colorSystem.GetColor(), layerSystem.GetLayer());

    canvas.ToggleLayerReload();
}

void BrushTool::OnButtonRelease() { }
void BrushTool::Render() { }
