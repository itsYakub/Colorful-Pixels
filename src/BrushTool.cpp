#include "BrushTool.hpp"

#include "raylib.h"

#include "Algorithms.hpp"
#include "Canvas.hpp"
#include "Project.hpp"
#include "ColorSystem.hpp"
#include "Cursor.hpp"

BrushTool::BrushTool(Project* project, ColorSystem* colorSystem) : 
    m_Project(project),
    m_ColorSystem(colorSystem) { }

void BrushTool::OnButtonPress() { }

void BrushTool::OnButtonDown() {
    int ax = m_Project->canvas->PositionAsCanvasIndex(m_Project->canvas->PositionInWorldSpace(GetMousePosition(), m_Project->camera)).x;
    int ay = m_Project->canvas->PositionAsCanvasIndex(m_Project->canvas->PositionInWorldSpace(GetMousePosition(), m_Project->camera)).y;

    int bx = m_Project->canvas->PositionAsCanvasIndex(m_Project->canvas->PositionInWorldSpace(m_Project->canvas->cursor.GetPreviousFramePosition(), m_Project->camera)).x;
    int by = m_Project->canvas->PositionAsCanvasIndex(m_Project->canvas->PositionInWorldSpace(m_Project->canvas->cursor.GetPreviousFramePosition(), m_Project->camera)).y;

    if(!m_Project->canvas->CanvasIndexValid({ static_cast<float>(ax), static_cast<float>(ay) }) || !m_Project->canvas->CanvasIndexValid({ static_cast<float>(bx), static_cast<float>(by) })) {
        return;
    }

    Algorithms::DigitalDifferentialAnalyzer(ax, ay, bx, by, m_ColorSystem->GetColor(), *m_Project->canvas->layerSystem.GetLayer());

    m_Project->canvas->ToggleTextureReload();
}

void BrushTool::OnButtonRelease() { }
void BrushTool::Render() { }
