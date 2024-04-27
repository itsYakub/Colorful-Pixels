#pragma once

#include <vector>
#include <memory>

#include "raylib.h"

#include "Viewport.hpp"
#include "LayerSystem.hpp"
#include "Cursor.hpp"

class Canvas {
public:
    bool drawGrid;
    bool drawFrame;
    bool drawCursor;
    
    float gridThickness;
    float frameThickness;
    float cursorThickness;

private:
    Viewport* m_Viewport;
    Camera2D m_Camera;
    Texture2D m_CanvasBackground;

    Vector2 m_Position;
    const Vector2 SIZE;

    const int CELL_COUNT_X;
    const int CELL_COUNT_Y;

    float m_Scale;

    bool m_ReloadLayerTexture;

    LayerSystem m_LayerSystem;
    Cursor m_Cursor;

public:
    Canvas(Viewport* viewport, const int CELL_COUNT_X, const int CELL_COUNT_Y);
    Canvas(Viewport* viewport);
    Canvas(Viewport* viewport, const int CELL_COUNT);

    void Unload();
    void Update();
    void Render();

    LayerSystem& GetLayerSystem();
    Cursor& GetCursor();

    const int CellCountX();
    const int CellCountY();

    void Pan();
    void Zoom();

    void ToggleTextureReload();

    Vector2 GetCanvasSize(const int COUNT_X, const int COUNT_Y);
    Vector2 GetCanvasOffset();

    void CenterCanvas();

    Vector2 PositionInWorldSpace(Vector2 position);
    Vector2 PositionAsCanvasCell(Vector2 position);
    Vector2 PositionAsCanvasIndex(Vector2 indexPosition);

    bool CanvasIndexValid(Vector2 position);

    void DrawBackground();
    void DrawLayer(bool visible, Layer& layer);
    void DrawCanvasGrid(const int WIDTH, const int HEIGHT);
    void DrawCanvasCursor();
    void DrawCanvasFrame();
};