#include "Cursor.hpp"

#include "raylib.h"

Cursor::Cursor() :
    m_PreviousFramePosition((Vector2) { 0.0f, 0.0f } ) { }

Vector2 Cursor::GetMousePositionWorld2D(Camera2D& camera) {
    return GetScreenToWorld2D(GetMousePosition(), camera);
}

Vector2 Cursor::GetPreviousFramePosition() {
    return m_PreviousFramePosition;
}

Vector2 Cursor::GetPreviousFramePositionWorld2D(Camera2D& camera) {
    return GetScreenToWorld2D(m_PreviousFramePosition, camera);
}

void Cursor::UpdatePreviousFramePosition() {
    m_PreviousFramePosition = GetMousePosition();
}