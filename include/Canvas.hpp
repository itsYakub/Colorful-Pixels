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

    // `DigitalDifferentialAnalyzer()` - algorithm for drawing lines in between two points on the 2D Raster.
    // Algorithm fill-up the blanks between points (`ax`, `ay`) and (`bx`, `by`) with the specified `color`
    //
    // NOTE: This function MUST use the index coordinates on the 2D grid; it fills the 2D grid based on the cell positions, not based on the mouse positions.
    // Example: 
    // Grid: 16x16
    // ax = 0, ay = 4
    // bx = 3, by = 15
    // These are the valid grid positions.
    void DigitalDifferentialAnalyzer(int ax, int ay, int bx, int by, Color color);

    Vector2 GetCanvasSize(const int COUNT_X, const int COUNT_Y);
    Vector2 GetCanvasOffset();

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