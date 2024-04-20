#pragma once

#include "Tool.hpp"

class Canvas;

class Eraser : public Tool {
private:
    Canvas& m_Canvas;

public:
    Eraser(Canvas& canvas);
    void OnButtonPress() override;
    void OnButtonDown() override;
    void OnButtonRelease() override;
};