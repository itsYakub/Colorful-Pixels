#pragma once

#include "raylib.h"

class Cursor {
private:
    Vector2 m_PreviousFramePosition;

public:
    Cursor();

    Vector2 GetMousePositionWorld2D(Camera2D& camera);

    Vector2 GetPreviousFramePosition();
    Vector2 GetPreviousFramePositionWorld2D(Camera2D& camera);
    void UpdatePreviousFramePosition();
};