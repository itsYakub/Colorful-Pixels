#pragma once

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
    Texture2D m_CanvasBackground;

    bool m_ReloadLayerTexture;

public:
    Canvas(const int CELL_COUNT_X, const int CELL_COUNT_Y, const LayerSystem& layerSystem);
    Canvas(const int CELL_COUNT_X, const int CELL_COUNT_Y);
    Canvas(const int CELL_COUNT, const LayerSystem& layerSystem);
    Canvas(const int CELL_COUNT);
    Canvas(const LayerSystem& layerSystem);
    Canvas();

    void Unload();
    void Update(Camera2D& camera, Vector2 viewportPosition);
    void Render(Camera2D& camera, Vector2 viewportPosition);

    void ToggleTextureReload();

    Vector2 PositionInWorldSpace(Vector2 screenspacePosition, Vector2 viewportPosition, Camera2D& camera);
    Vector2 PositionAsCanvasCell(Vector2 worldspacePosition);
    Vector2 PositionAsCanvasIndex(Vector2 worldspacePosition);

    bool CanvasIndexValid(Vector2 index);

    void DrawBackground();
    void DrawLayer(bool visible, Layer& layer);
    void DrawCell(float x, float y, Color color);
    void DrawCellLines(float x, float y, float thickness, Color color);
    void DrawCanvasGrid();
    void DrawCanvasCursor(Camera2D& camera, Vector2 viewportPosition);
    void DrawCanvasFrame();

    Vector2 GetCanvasSize(const int COUNT_X, const int COUNT_Y);
    Vector2 GetCellSize();
    Vector2 GetCanvasOffset();
};