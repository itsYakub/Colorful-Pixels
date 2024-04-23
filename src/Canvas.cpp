#include "Canvas.hpp"

#include <cmath>
#include <utils.h>

#include "raylib.h"
#include "raymath.h"

#include "Cursor.hpp"
#include "Viewport.hpp"

Canvas::Canvas(Viewport* viewport, const int CELL_COUNT_X, const int CELL_COUNT_Y) : 
    drawGrid(false),
    drawFrame(true),
    drawCursor(true),
    gridThickness(1.0f),
    frameThickness(4.0f),
    cursorThickness(2.0f),

    m_Viewport(viewport),
    m_Camera(),

    m_Position(Vector2Zero()), 
    SIZE(GetCanvasSize(CELL_COUNT_X, CELL_COUNT_Y)),

    CELL_COUNT_X(CELL_COUNT_X), 
    CELL_COUNT_Y(CELL_COUNT_Y),

    m_Scale(1.0f),

    m_LayerSystem(this),
    m_Cursor() {
        m_Camera.target = m_Position;
        m_Camera.offset = { 64.0f, 64.0f };
        m_Camera.zoom = m_Scale;

        Image canvasBackgroundImage = GenImageChecked(SIZE.x, SIZE.y, 64, 64, (Color) { 128, 128, 128, 255 }, (Color) { 192, 192, 192, 255 });
        m_CanvasBackground = LoadTextureFromImage(canvasBackgroundImage);
        UnloadImage(canvasBackgroundImage);
}

Canvas::Canvas(Viewport* viewport) : Canvas(viewport, 32, 32) { }
Canvas::Canvas(Viewport* viewport, const int CELL_COUNT) : Canvas(viewport, CELL_COUNT, CELL_COUNT) { }

void Canvas::Unload() {
    m_LayerSystem.Unload();
    UnloadTexture(m_CanvasBackground);
}

void Canvas::Update() {
    m_LayerSystem.UpdateLayer();
    m_Cursor.UpdatePreviousFramePosition();
}

void Canvas::Render() {    
    BeginMode2D(m_Camera);

        DrawBackground();

        for(int i = m_LayerSystem.GetCount() - 1; i >= 0; i--) {
            DrawLayer(m_LayerSystem.GetLayer(i)->IsVisible(), *m_LayerSystem.GetLayer(i));
        }

        DrawCanvasCursor();
        DrawCanvasGrid(CELL_COUNT_X, CELL_COUNT_Y);
        DrawCanvasFrame();

    EndMode2D();
}

LayerSystem& Canvas::GetLayerSystem() {
    return m_LayerSystem;
}

Cursor& Canvas::GetCursor() {
    return m_Cursor;
}

const int Canvas::CellCountX() {
    return CELL_COUNT_X;
}

const int Canvas::CellCountY() {
    return CELL_COUNT_Y;
}

void Canvas::Pan() {
    Vector2 delta = GetMouseDelta();
    delta = Vector2Scale(delta, -1.0f / m_Camera.zoom);

    m_Camera.target = Vector2Add(m_Camera.target, delta);
}

void Canvas::Zoom() {
    m_Scale += GetMouseWheelMove() / 10.0f;
    m_Scale = Clamp(m_Scale, 0.5f, 4.0f);
    
    m_Camera.target = GetScreenToWorld2D(GetMousePosition(), m_Camera);
    m_Camera.offset = GetMousePosition();
    m_Camera.zoom = m_Scale;
}

// `DigitalDifferentialAnalyzer()` - algorithm for drawing lines in between two points on the 2D Raster.
// Algorithm fill-up the blanks between points (`ax`, `ay`) and (`bx`, `by`) with the specified `color`
//
// NOTE: This function MUST use the index coordinates on the 2D grid; it fills the 2D grid based on the cell positions, not based on the mouse positions.
// Example: 
// Grid: 16x16
// ax = 0, ay = 4
// bx = 3, by = 15
// These are the valid grid positions.
void Canvas::DigitalDifferentialAnalyzer(int ax, int ay, int bx, int by, Color color) {
    // source: https://en.wikipedia.org/wiki/Digital_differential_analyzer_(graphics_algorithm)

    float dx = bx - ax;
    float dy = by - ay;
    float steps = 0;
    float i = 0;

    float x = 0;
    float y = 0;

    if (abs(dx) >= abs(dy)) {
        steps = abs(dx);
    } else {
        steps = abs(dy);
    }

    dx = dx / steps;
    dy = dy / steps;
    x = ax;
    y = ay;

    while (i <= steps) {
        m_LayerSystem.GetLayer()->SetPixelColor(std::floor(x), std::floor(y), color);
        x += dx;
        y += dy;
        i++;
    }
}

Vector2 Canvas::GetCanvasSize(const int COUNT_X, const int COUNT_Y) {
    const float defaultResolutionValue = 512.0f;
    Vector2 result = { 0.0f, 0.0f };

    if(COUNT_X > COUNT_Y) {
        result = { 
            defaultResolutionValue, 
            (defaultResolutionValue / COUNT_X) * COUNT_Y 
        };
    } else if(COUNT_X < COUNT_Y) {
        result = { 
            (defaultResolutionValue / COUNT_Y) * COUNT_X, 
            defaultResolutionValue 
        };
    } else {
        result = { 
            defaultResolutionValue, 
            defaultResolutionValue 
        };
    }

    TraceLog(LOG_INFO, TextFormat("Canvas size: %0.1f / %0.1f (%ix%i)", result.x, result.y, COUNT_X, COUNT_Y));

    return result;
}

// TODO: Implement this later
Vector2 Canvas::GetCanvasOffset() {
    return {
        SIZE.x / m_Scale / 2.0f,
        SIZE.y / m_Scale / 2.0f
    };
}


Vector2 Canvas::PositionInWorldSpace(Vector2 position) {
    return {
        GetScreenToWorld2D({ position.x - m_Viewport->GetPosition().x, position.y - m_Viewport->GetPosition().y - 20 }, m_Camera).x,
        GetScreenToWorld2D({ position.x - m_Viewport->GetPosition().x, position.y - m_Viewport->GetPosition().y - 20 }, m_Camera).y
    };
}

Vector2 Canvas::PositionAsCanvasCell(Vector2 position) {
    return {
        PositionAsCanvasIndex(PositionInWorldSpace(position)).x * (SIZE.x / CELL_COUNT_X),
        PositionAsCanvasIndex(PositionInWorldSpace(position)).y * (SIZE.y / CELL_COUNT_Y)
    };
}

Vector2 Canvas::PositionAsCanvasIndex(Vector2 position) {
    return {
        std::floor(((position.x / m_Scale)) / SIZE.x * CELL_COUNT_X),
        std::floor(((position.y / m_Scale)) / SIZE.y * CELL_COUNT_X)
    };
}

bool Canvas::CanvasIndexValid(Vector2 indexPosition) {
    return 
        indexPosition.x >= 0 && indexPosition.x < CELL_COUNT_X &&
        indexPosition.y >= 0 && indexPosition.y < CELL_COUNT_Y; 
}

void Canvas::DrawBackground() {
    DrawTexturePro(
        m_CanvasBackground,
        (Rectangle) { 
            0.0f, 
            0.0f, 
            static_cast<float>(m_CanvasBackground.width), 
            static_cast<float>(m_CanvasBackground.height) 
        },
        (Rectangle) { 
            m_Position.x, 
            m_Position.y, 
            SIZE.x * m_Scale, 
            SIZE.y * m_Scale 
        },
        Vector2Zero(),
        0.0f,
        WHITE
    ); 
}

void Canvas::DrawLayer(bool visible, Layer& layer) {
    if(!visible) {
        return;
    }

    DrawTexturePro(
        layer.GetTexture(),
        (Rectangle) { 
            0.0f, 
            0.0f, 
            static_cast<float>(CELL_COUNT_X), 
            static_cast<float>(CELL_COUNT_Y) 
        },
        (Rectangle) { 
            m_Position.x, 
            m_Position.y, 
            SIZE.x * m_Scale, 
            SIZE.y * m_Scale 
        },
        Vector2Zero(),
        0.0f,
        WHITE
    );    
}

void Canvas::DrawCanvasGrid(const int WIDTH, const int HEIGHT) {
    if(!drawGrid) {
        return;
    }

    for(int y = 0; y < HEIGHT; y++) {
        for(int x = 0; x < WIDTH; x++) {
            DrawRectangleLinesEx(
                (Rectangle) { 
                    m_Position.x + (x * (SIZE.x * m_Scale / WIDTH)),
                    m_Position.y + (y * (SIZE.y * m_Scale / HEIGHT)),
                    SIZE.x * m_Scale / WIDTH, 
                    SIZE.y * m_Scale / HEIGHT 
                }, 
                gridThickness / m_Scale, 
                Fade(BLACK, 0.5f)
            );
        }
    }        
}

void Canvas::DrawCanvasCursor() {
    if(!drawCursor && m_Viewport->IsCursorInViewport()) {
        return;
    }

    DrawRectangleLinesEx(
        (Rectangle) {
            PositionAsCanvasCell(GetMousePosition()).x * m_Scale,
            PositionAsCanvasCell(GetMousePosition()).y * m_Scale,
            SIZE.x * m_Scale / CELL_COUNT_X, 
            SIZE.y * m_Scale / CELL_COUNT_Y 
        },
        cursorThickness / m_Scale,
        WHITE
    );
}

void Canvas::DrawCanvasFrame() {
    if(!drawFrame) {
        return;
    }

    DrawRectangleLinesEx(
        (Rectangle) {
            m_Position.x,
            m_Position.y,
            SIZE.x * m_Scale,
            SIZE.y * m_Scale
        },
        frameThickness / m_Scale,
        BLACK
    );
}
