#pragma once

#include "Tool.hpp"

class Project;

class PanTool : public Tool {
private:
    Project* m_Project;

public:
    PanTool(Project* project);
    void OnButtonPress() override;
    void OnButtonDown() override;
    void OnButtonRelease() override;
    void Render() override;
};