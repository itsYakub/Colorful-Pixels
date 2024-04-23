#pragma once

#include "Tool.hpp"

class Canvas;
class Cursor;

class EraserTool : public Tool {
private:
    Canvas* m_Canvas;
    Cursor* m_Cursor;

public:
    EraserTool(Canvas* canvas, Cursor* cursor);
    void OnButtonPress() override;
    void OnButtonDown() override;
    void OnButtonRelease() override;
};