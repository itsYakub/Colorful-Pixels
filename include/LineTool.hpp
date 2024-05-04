#pragma once

#include "raylib.h"

#include "Tool.hpp"

class Project;
class ColorSystem;

class LineTool : public Tool {
private:
    Project* m_Project;

    Vector2 m_PointA;
    Vector2 m_PointB;

public:
    LineTool(int id, Project* project);
    void OnButtonPress() override;
    void OnButtonDown() override;
    void OnButtonRelease() override;
    void Render() override;
};