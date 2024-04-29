#pragma once

#include "Tool.hpp"

class Project;
class ColorSystem;

class BrushTool : public Tool {
private:
    Project* m_Project;
    ColorSystem* m_ColorSystem;

public:
    BrushTool(Project* project, ColorSystem* colorSystem);
    void OnButtonPress() override;
    void OnButtonDown() override;
    void OnButtonRelease() override;
    void Render() override;
};