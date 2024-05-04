#include "ColorPickerTool.hpp"

#include "raylib.h"

#include "Canvas.hpp"
#include "Project.hpp"
#include "ColorSystem.hpp"
#include "ToolSystem.hpp"
#include "BrushTool.hpp"


ColorPickerTool::ColorPickerTool(int id, Project* project) : 
    m_Project(project) { ID = id; }

void ColorPickerTool::OnButtonPress() { 
    Canvas& canvas = *m_Project->canvas;
    Viewport& viewport = m_Project->viewport;
    LayerSystem& layerSystem = canvas.layerSystem;
    ColorSystem& colorSystem = m_Project->colorSystem;
    ToolSystem& toolSystem = m_Project->toolSystem;

    int x = canvas.PositionAsCanvasIndex(canvas.PositionInWorldSpace(GetMousePosition(), viewport.GetPosition(), m_Project->camera)).x;
    int y = canvas.PositionAsCanvasIndex(canvas.PositionInWorldSpace(GetMousePosition(), viewport.GetPosition(), m_Project->camera)).y;

    if(ColorToInt(layerSystem.GetLayer()->GetPixelColor(x, y)) != ColorToInt(BLANK)) {
        colorSystem.SetColor(layerSystem.GetLayer()->GetPixelColor(x, y));
        toolSystem.SetCurrentTool(ToolSystem::TOOL_BRUSH, *m_Project);
    }
}

void ColorPickerTool::OnButtonDown() { }

void ColorPickerTool::OnButtonRelease() { }

void ColorPickerTool::Render() { }
