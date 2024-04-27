#pragma once

#include "Tool.hpp"

class Canvas;

class PanTool : public Tool {
private:
    Canvas* m_Canvas;

public:
    PanTool(Canvas* canvas);
    void OnButtonPress() override;
    void OnButtonDown() override;
    void OnButtonRelease() override;
    void Render() override;
};