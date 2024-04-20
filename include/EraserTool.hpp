#pragma once

#include "Tool.hpp"

class Canvas;

class EraserTool : public Tool {
private:
    Canvas* m_Canvas;

public:
    EraserTool(Canvas* canvas);
    void OnButtonPress() override;
    void OnButtonDown() override;
    void OnButtonRelease() override;
};