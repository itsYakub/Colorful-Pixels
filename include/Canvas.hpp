#pragma once

#include <vector>
#include <memory>

#include "raylib.h"

#include "Viewport.hpp"
#include "LayerSystem.hpp"
#include "Cursor.hpp"

class Canvas {
public:
    LayerSystem layerSystem;
    Cursor cursor;

    const Vector2 SIZE;

    const int CELL_COUNT_X;
    const int CELL_COUNT_Y;

    float scale;

private:
    Viewport* m_Viewport;
    Texture2D m_CanvasBackground;

    bool m_ReloadLayerTexture;

public:
    Canvas(Viewport* viewport, const int CELL_COUNT_X, const int CELL_COUNT_Y);
    Canvas(Viewport* viewport);
    Canvas(Viewport* viewport, const int CELL_COUNT);

    void Unload();
    void Update();
    void Render(Camera2D& camera);

    void ToggleTextureReload();

    Vector2 PositionInWorldSpace(Vector2 screenspacePosition, Camera2D camera);
    Vector2 PositionAsCanvasCell(Vector2 worldspacePosition);
    Vector2 PositionAsCanvasIndex(Vector2 worldspacePosition);

    bool CanvasIndexValid(Vector2 index);

    void DrawBackground();
    void DrawLayer(bool visible, Layer& layer);
    void DrawCanvasGrid(const int WIDTH, const int HEIGHT);
    void DrawCanvasCursor(Camera2D& camera);
    void DrawCanvasFrame();

    Vector2 GetCanvasSize(const int COUNT_X, const int COUNT_Y);
    Vector2 GetCanvasOffset();
};