#include "Canvas.hpp"

#include <cmath>
#include <utils.h>

#include "raylib.h"
#include "raymath.h"

#include "Cursor.hpp"
#include "Viewport.hpp"

Canvas::Canvas(Viewport* viewport, const int CELL_COUNT_X, const int CELL_COUNT_Y) : 
    m_Viewport(viewport),
    m_Camera(),

    m_Position(Vector2Zero()), 
    SIZE(GetCanvasSize(CELL_COUNT_X, CELL_COUNT_Y)),

    CELL_COUNT_X(CELL_COUNT_X), 
    CELL_COUNT_Y(CELL_COUNT_Y),

    m_Scale(1.0f),

    m_ReloadLayerTexture(false),

    m_LayerSystem(this),
    m_Cursor() {
        m_Camera.target = m_Position;
        m_Camera.offset = { 512.0f, 128.0f };
        m_Camera.zoom = m_Scale;

        Image canvasBackgroundImage = GenImageChecked(SIZE.x, SIZE.y, 128, 128, (Color) { 128, 128, 128, 255 }, (Color) { 192, 192, 192, 255 });
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
    m_Cursor.UpdatePreviousFramePosition();

    if(m_ReloadLayerTexture) {
        m_LayerSystem.UpdateLayer();
        m_ReloadLayerTexture = false;
    }
}

void Canvas::Render() {    
    BeginMode2D(m_Camera);

        DrawBackground();

        for(int i = m_LayerSystem.GetCount() - 1; i >= 0; i--) {
            DrawLayer(m_LayerSystem.GetLayer(i)->IsVisible(), *m_LayerSystem.GetLayer(i));
        }

        DrawCanvasGrid(CELL_COUNT_X, CELL_COUNT_Y);
        DrawCanvasCursor();
        DrawCanvasFrame();

    EndMode2D();
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

void Canvas::ToggleTextureReload() {
    m_ReloadLayerTexture = true;
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

void Canvas::CenterCanvas() {
    m_Camera.offset = {
        m_Viewport->GetSize().x - GetCanvasOffset().x / 2.0f,
        m_Viewport->GetSize().y - GetCanvasOffset().y / 2.0f
    };
}


Vector2 Canvas::PositionInWorldSpace(Vector2 screenspacePosition) {
    return {
        GetScreenToWorld2D({ screenspacePosition.x - m_Viewport->GetPosition().x, screenspacePosition.y - m_Viewport->GetPosition().y }, m_Camera).x,
        GetScreenToWorld2D({ screenspacePosition.x - m_Viewport->GetPosition().x, screenspacePosition.y - m_Viewport->GetPosition().y }, m_Camera).y
    };
}

Vector2 Canvas::PositionAsCanvasCell(Vector2 worldspacePosition) {
    return {
        PositionAsCanvasIndex(PositionInWorldSpace(worldspacePosition)).x * (SIZE.x / CELL_COUNT_X),
        PositionAsCanvasIndex(PositionInWorldSpace(worldspacePosition)).y * (SIZE.y / CELL_COUNT_Y)
    };
}

Vector2 Canvas::PositionAsCanvasIndex(Vector2 worldspacePosition) {
    return {
        std::floor(((worldspacePosition.x / m_Scale)) / SIZE.x * CELL_COUNT_X),
        std::floor(((worldspacePosition.y / m_Scale)) / SIZE.y * CELL_COUNT_X)
    };
}

bool Canvas::CanvasIndexValid(Vector2 index) {
    return 
        index.x >= 0 && index.x < CELL_COUNT_X &&
        index.y >= 0 && index.y < CELL_COUNT_Y; 
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
    for(int y = 0; y < HEIGHT; y++) {
        for(int x = 0; x < WIDTH; x++) {
            DrawRectangleLinesEx(
                (Rectangle) { 
                    m_Position.x + (x * (SIZE.x * m_Scale / WIDTH)),
                    m_Position.y + (y * (SIZE.y * m_Scale / HEIGHT)),
                    SIZE.x * m_Scale / WIDTH, 
                    SIZE.y * m_Scale / HEIGHT 
                }, 
                1.0 / m_Scale, 
                Fade(BLACK, 0.5f)
            );
        }
    }        
}

void Canvas::DrawCanvasCursor() {
    if(!m_Viewport->IsCursorInViewport()) {
        return;
    }

    DrawRectangleLinesEx(
        (Rectangle) {
            PositionAsCanvasCell(GetMousePosition()).x * m_Scale,
            PositionAsCanvasCell(GetMousePosition()).y * m_Scale,
            SIZE.x * m_Scale / CELL_COUNT_X, 
            SIZE.y * m_Scale / CELL_COUNT_Y 
        },
        2.0f / m_Scale,
        WHITE
    );
}

void Canvas::DrawCanvasFrame() {
    DrawRectangleLinesEx(
        (Rectangle) {
            m_Position.x,
            m_Position.y,
            SIZE.x * m_Scale,
            SIZE.y * m_Scale
        },
        4.0f / m_Scale,
        BLACK
    );
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

