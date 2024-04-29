#include "Canvas.hpp"

#include <cmath>
#include <utils.h>

#include "LayerSystem.hpp"
#include "raylib.h"
#include "raymath.h"

#include "Cursor.hpp"
#include "Viewport.hpp"

Canvas::Canvas(Viewport* viewport, const int CELL_COUNT_X, const int CELL_COUNT_Y) : 
    layerSystem(CELL_COUNT_X, CELL_COUNT_Y),
    cursor(),
    SIZE(GetCanvasSize(CELL_COUNT_X, CELL_COUNT_Y)),
    CELL_COUNT_X(CELL_COUNT_X),
    CELL_COUNT_Y(CELL_COUNT_Y),
    scale(1.0f),
    m_Viewport(viewport),
    m_ReloadLayerTexture(false) {
        Image canvasBackgroundImage = GenImageChecked(SIZE.x, SIZE.y, SIZE.x / 2.0f, SIZE.x / 2.0f, (Color) { 128, 128, 128, 255 }, (Color) { 192, 192, 192, 255 });
        m_CanvasBackground = LoadTextureFromImage(canvasBackgroundImage);
        UnloadImage(canvasBackgroundImage);
}

Canvas::Canvas(Viewport* viewport) : Canvas(viewport, 32, 32) { }
Canvas::Canvas(Viewport* viewport, const int CELL_COUNT) : Canvas(viewport, CELL_COUNT, CELL_COUNT) { }

void Canvas::Unload() {
    layerSystem.Unload();
    UnloadTexture(m_CanvasBackground);
}

void Canvas::Update() {
    cursor.UpdatePreviousFramePosition();

    if(m_ReloadLayerTexture) {
        layerSystem.UpdateLayer();
        m_ReloadLayerTexture = false;
    }
}

void Canvas::Render(Camera2D& camera) {    
    DrawBackground();

    for(int i = layerSystem.GetCount() - 1; i >= 0; i--) {
        DrawLayer(layerSystem.GetLayer(i)->IsVisible(), *layerSystem.GetLayer(i));
    }

    // DrawCanvasGrid(CELL_COUNT_X, CELL_COUNT_Y);
    DrawCanvasCursor(camera);
    DrawCanvasFrame();
}

void Canvas::ToggleTextureReload() {
    m_ReloadLayerTexture = true;
}

Vector2 Canvas::GetCanvasSize(const int COUNT_X, const int COUNT_Y) {
    const float DEFAULT_SIZE = 512.0f;
    Vector2 result = Vector2Zero();

    if(COUNT_X > COUNT_Y) {
        result = { 
            DEFAULT_SIZE, 
            (DEFAULT_SIZE / COUNT_X) * COUNT_Y 
        };
    } else if(COUNT_X < COUNT_Y) {
        result = { 
            (DEFAULT_SIZE / COUNT_Y) * COUNT_X, 
            DEFAULT_SIZE 
        };
    } else {
        result = { 
            DEFAULT_SIZE, 
            DEFAULT_SIZE 
        };
    }

    TraceLog(LOG_INFO, TextFormat("Canvas size: %0.1f / %0.1f (%ix%i)", result.x, result.y, COUNT_X, COUNT_Y));

    return result;
}

Vector2 Canvas::PositionInWorldSpace(Vector2 screenspacePosition, Camera2D camera) {
    return {
        GetScreenToWorld2D({ screenspacePosition.x - m_Viewport->GetPosition().x, screenspacePosition.y - m_Viewport->GetPosition().y }, camera).x,
        GetScreenToWorld2D({ screenspacePosition.x - m_Viewport->GetPosition().x, screenspacePosition.y - m_Viewport->GetPosition().y }, camera).y
    };
}

Vector2 Canvas::PositionAsCanvasCell(Vector2 worldspacePosition) {
    return {
        PositionAsCanvasIndex(worldspacePosition).x * (SIZE.x / CELL_COUNT_X),
        PositionAsCanvasIndex(worldspacePosition).y * (SIZE.y / CELL_COUNT_Y)
    };
}

Vector2 Canvas::PositionAsCanvasIndex(Vector2 worldspacePosition) {
    return {
        std::floor(((worldspacePosition.x / scale)) / SIZE.x * CELL_COUNT_X),
        std::floor(((worldspacePosition.y / scale)) / SIZE.y * CELL_COUNT_Y)
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
            0.0f, 
            0.0f, 
            SIZE.x * scale, 
            SIZE.y * scale 
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
            0.0f, 
            0.0f, 
            SIZE.x * scale, 
            SIZE.y * scale 
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
                    x * (SIZE.x * scale / WIDTH),
                    y * (SIZE.y * scale / HEIGHT),
                    SIZE.x * scale / WIDTH, 
                    SIZE.y * scale / HEIGHT 
                }, 
                1.0 / scale, 
                Fade(BLACK, 0.5f)
            );
        }
    }        
}

void Canvas::DrawCanvasCursor(Camera2D& camera) {
    if(!m_Viewport->IsCursorInViewport()) {
        return;
    }

    DrawRectangleLinesEx(
        (Rectangle) {
            PositionAsCanvasCell(PositionInWorldSpace(GetMousePosition(), camera)).x * scale,
            PositionAsCanvasCell(PositionInWorldSpace(GetMousePosition(), camera)).y * scale,
            SIZE.x * scale / CELL_COUNT_X, 
            SIZE.y * scale / CELL_COUNT_Y 
        },
        2.0f / scale,
        WHITE
    );
}

void Canvas::DrawCanvasFrame() {
    DrawRectangleLinesEx(
        (Rectangle) {
            0.0f,
            0.0f,
            SIZE.x * scale,
            SIZE.y * scale
        },
        2.0f / scale,
        BLACK
    );
}