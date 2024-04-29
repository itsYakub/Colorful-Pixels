#include "FillTool.hpp"

#include "raylib.h"

#include "Algorithms.hpp"
#include "Project.hpp"
#include "Canvas.hpp"
#include "ColorSystem.hpp"
#include "Cursor.hpp"

FillTool::FillTool(Project* Project, ColorSystem* colorSystem) : 
    m_Project(Project),
    m_ColorSystem(colorSystem) { }

void FillTool::OnButtonPress() {
    int x = m_Project->canvas->PositionAsCanvasIndex(m_Project->canvas->PositionInWorldSpace(GetMousePosition(), m_Project->camera)).x;
    int y = m_Project->canvas->PositionAsCanvasIndex(m_Project->canvas->PositionInWorldSpace(GetMousePosition(), m_Project->camera)).y;

    if(!m_Project->canvas->CanvasIndexValid({ static_cast<float>(x), static_cast<float>(y) })) {
        return;
    }

    Algorithms::FloodFillAlgorithm(x, y, m_Project->canvas->layerSystem.GetLayer()->GetPixelColor(x, y), m_ColorSystem->GetColor(), *m_Project->canvas->layerSystem.GetLayer());

    m_Project->canvas->ToggleTextureReload();
}

void FillTool::OnButtonDown() { }

void FillTool::OnButtonRelease() { }

void FillTool::Render() { }
