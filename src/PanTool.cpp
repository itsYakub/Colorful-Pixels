#include "PanTool.hpp"

#include "Canvas.hpp"

PanTool::PanTool(Canvas* canvas) : 
    m_Canvas(canvas) { }

void PanTool::OnButtonPress() { }

void PanTool::OnButtonDown() {
    m_Canvas->Pan();
}

void PanTool::OnButtonRelease() { }

void PanTool::Render() { }
