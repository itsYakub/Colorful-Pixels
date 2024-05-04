#pragma once

#include "Tool.hpp"

class Project;
class ColorSystem;

class FillTool : public Tool {
private:
    Project* m_Project;

public:
    FillTool(int id, Project* project);
    void OnButtonPress() override;
    void OnButtonDown() override;
    void OnButtonRelease() override;
    void Render() override;
};