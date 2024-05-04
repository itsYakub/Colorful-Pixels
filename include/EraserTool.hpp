#pragma once

#include "Tool.hpp"

class Project;
class Cursor;

class EraserTool : public Tool {
private:
    Project* m_Project;

public:
    EraserTool(int id, Project* project);
    void OnButtonPress() override;
    void OnButtonDown() override;
    void OnButtonRelease() override;
    void Render() override;
};