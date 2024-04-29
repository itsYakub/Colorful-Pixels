#include "EraserTool.hpp"

#include "raylib.h"

#include "Algorithms.hpp"
#include "Project.hpp"
#include "Canvas.hpp"
#include "Cursor.hpp"

EraserTool::EraserTool(Project* project) : 
    m_Project(project) { }

void EraserTool::OnButtonPress() { }

void EraserTool::OnButtonDown() {
    int ax = m_Project->canvas->PositionAsCanvasIndex(m_Project->canvas->PositionInWorldSpace(GetMousePosition(), m_Project->camera)).x;
    int ay = m_Project->canvas->PositionAsCanvasIndex(m_Project->canvas->PositionInWorldSpace(GetMousePosition(), m_Project->camera)).y;

    int bx = m_Project->canvas->PositionAsCanvasIndex(m_Project->canvas->PositionInWorldSpace(m_Project->canvas->cursor.GetPreviousFramePosition(), m_Project->camera)).x;
    int by = m_Project->canvas->PositionAsCanvasIndex(m_Project->canvas->PositionInWorldSpace(m_Project->canvas->cursor.GetPreviousFramePosition(), m_Project->camera)).y;

    if(!m_Project->canvas->CanvasIndexValid({ static_cast<float>(ax), static_cast<float>(ay) }) || !m_Project->canvas->CanvasIndexValid({ static_cast<float>(bx), static_cast<float>(by) })) {
        return;
    }

    Algorithms::DigitalDifferentialAnalyzer(ax, ay, bx, by, BLANK, *m_Project->canvas->layerSystem.GetLayer());

    m_Project->canvas->ToggleTextureReload();
}

void EraserTool::OnButtonRelease() { }

void EraserTool::Render() { }

