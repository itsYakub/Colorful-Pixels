#pragma once

#include "Tool.hpp"

class Canvas;

class Brush : public Tool {
private:
    Canvas& m_Canvas;

public:
    Brush(Canvas& canvas);
    void OnButtonPress() override;
    void OnButtonDown() override;
    void OnButtonRelease() override;
};