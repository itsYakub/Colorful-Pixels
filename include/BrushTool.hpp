#pragma once

#include "Tool.hpp"

class Project;

class BrushTool : public Tool {
private:
    Project* m_Project;

public:
    BrushTool(int id, Project* project);
    void OnButtonPress() override;
    void OnButtonDown() override;
    void OnButtonRelease() override;
    void Render() override;
};