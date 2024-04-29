#pragma once

#include "Tool.hpp"

class Project;
class ColorSystem;

class FillTool : public Tool {
private:
    Project* m_Project;
    ColorSystem* m_ColorSystem;

public:
    FillTool(Project* project, ColorSystem* colorSystem);
    void OnButtonPress() override;
    void OnButtonDown() override;
    void OnButtonRelease() override;
    void Render() override;
};