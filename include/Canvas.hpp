#pragma once

#include <vector>
#include <memory>

#include "raylib.h"

#include "Viewport.hpp"
#include "Layer.hpp"
#include "ColorPalette.hpp"
#include "Tool.hpp"
#include "Brush.hpp"
#include "Eraser.hpp"

class Canvas {
private:
    Viewport* m_Viewport;
    Camera2D m_Camera;
    const Vector2 m_CameraOffset;

    Vector2 m_PreviousFrameMousePosition;
    Vector2 m_Position;
    const Vector2 SIZE;

    const int CELL_COUNT_X;
    const int CELL_COUNT_Y;

    float m_Scale;

    std::vector<std::unique_ptr<Layer>> m_LayerList;
    int m_CurrentLayerID;

    ColorPalette m_ColorPalette;
    Color m_CurrentColor;

    std::unique_ptr<Tool> m_CurrentTool;

public:
    Canvas(Viewport* viewport, const int CELL_COUNT_X, const int CELL_COUNT_Y);
    Canvas(Viewport* viewport);
    Canvas(Viewport* viewport, const int CELL_COUNT);

    void Unload();
    void Update();
    void Render();

    void ColorGuiPanel(const char* name, ImVec2 position, ImVec2 size);
    void LayersGuiPanel(const char* name, ImVec2 position, ImVec2 size);
    void ToolsGuiPanel(const char* name, ImVec2 position, ImVec2 size);

    std::unique_ptr<Layer>& GetLayer();
    std::unique_ptr<Layer>& GetLayer(int index);

    Color GetColor();
    void SetColor(Color color);

    // `GetMouseCanvasIndex()` - This function returns the cell index of the canvas that the cursor is currently hovering above
    Vector2 GetMouseCanvasIndex(Vector2 mousePositionScaled);

    // `GetMouseWorldPosition()` - This function returns the position of the mouse cursor in the 2D World space ( `GetScreenToWorld2D()` )
    Vector2 MouseWorldPosition();

    // `GetMouseWorldPositionScaled()` - This function returns the position of the mouse cursor in the 2D World space divided by the scale factor ( `scale` )
    Vector2 MouseWorldPositionScaled();

    Vector2& PreviousFrameMousePosition();

    Vector2 PreviousFrameMousePositionScaled();

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

private:
    // `GetCanvasSize()` - This function returns the size of the canvas based on the count of the cells in X and Y axis (default resolution is 512px)
    Vector2 GetCanvasSize(const int COUNT_X, const int COUNT_Y);

    // `GetCanvasOriginOffset()` - This function returns the origin offset from the center of the canvas
    Vector2 GetCanvasOriginOffset();

    // `GetMouseWorldCanvasPosition()` - This function returns the position of the cell in the canvas, based on the current mouse position in the 2D World ( `GetMouseWorldPosition()` )
    Vector2 GetMouseToCanvasCellPosition(Vector2 mouseCanvasIndex);

    // `IsMouseCanvasIndexValid()` - This function check's if `GetMouseCanvasIndex()` returns a valid index for the `positionScaled` mouse position
    bool IsMouseCanvasIndexValid(Vector2 positionScaled);

    bool MouseViewportHover();

    void DrawLayer(bool visible, Layer& layer);
    void DrawCanvasGrid(bool visible, const int WIDTH, const int HEIGHT);
    void DrawCanvasCursor(bool visible);
    void DrawCanvasFrame(bool visible);
};