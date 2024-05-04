#pragma once

#include "Tool.hpp"

class Project;
class ColorSystem;

class ColorPickerTool : public Tool {
private:
    Project* m_Project;

public:
    ColorPickerTool(int id, Project* project);
    void OnButtonPress() override;
    void OnButtonDown() override;
    void OnButtonRelease() override;
    void Render() override;
};