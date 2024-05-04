#include "FillTool.hpp"

#include "raylib.h"

#include "Algorithms.hpp"
#include "Project.hpp"
#include "Canvas.hpp"
#include "ColorSystem.hpp"

FillTool::FillTool(int id, Project* project) : 
    m_Project(project){ ID = id; }

void FillTool::OnButtonPress() {
    Canvas& canvas = *m_Project->canvas;
    Viewport& viewport = m_Project->viewport;
    LayerSystem& layerSystem = canvas.layerSystem;
    ColorSystem& colorSystem = m_Project->colorSystem;

    int x = canvas.PositionAsCanvasIndex(canvas.PositionInWorldSpace(GetMousePosition(), viewport.GetPosition(), m_Project->camera)).x;
    int y = canvas.PositionAsCanvasIndex(canvas.PositionInWorldSpace(GetMousePosition(), viewport.GetPosition(), m_Project->camera)).y;

    if(!canvas.CanvasIndexValid({ static_cast<float>(x), static_cast<float>(y) })) {
        return;
    }

    Algorithms::FloodFillAlgorithm(x, y, layerSystem.GetLayer().GetPixelColor(x, y), colorSystem.GetColor(), layerSystem.GetLayer());

    canvas.ToggleTextureReload();
}

void FillTool::OnButtonDown() { }

void FillTool::OnButtonRelease() { }

void FillTool::Render() { }
