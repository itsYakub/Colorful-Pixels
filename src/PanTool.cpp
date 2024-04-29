#include "PanTool.hpp"

#include "Canvas.hpp"
#include "Project.hpp"

PanTool::PanTool(Project* project) : 
    m_Project(project) { }

void PanTool::OnButtonPress() { }

void PanTool::OnButtonDown() {
    m_Project->Pan();
}

void PanTool::OnButtonRelease() { }

void PanTool::Render() { }
