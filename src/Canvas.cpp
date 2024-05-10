#include "Canvas.hpp"

#include <cmath>
#include <utils.h>

#include "LayerSystem.hpp"
#include "raylib.h"
#include "raymath.h"

#include "Cursor.hpp"
#include "Viewport.hpp"

Canvas::Canvas(const int CELL_COUNT_X, const int CELL_COUNT_Y, const LayerSystem& layerSystem) :     
    layerSystem(layerSystem),
    cursor(),
    SIZE(GetCanvasSize(CELL_COUNT_X, CELL_COUNT_Y)),
    CELL_COUNT_X(CELL_COUNT_X),
    CELL_COUNT_Y(CELL_COUNT_Y),
    scale(1.0f),
    m_ReloadLayerTexture(false) {
        Image canvasBackgroundImage = GenImageChecked(SIZE.x, SIZE.y, 128, 128, (Color) { 128, 128, 128, 255 }, (Color) { 192, 192, 192, 255 });
        m_CanvasBackground = LoadTextureFromImage(canvasBackgroundImage);
        UnloadImage(canvasBackgroundImage);

        TraceLog(LOG_INFO, "CANVAS: Canvas created successfully.");
}

Canvas::Canvas(const int CELL_COUNT_X, const int CELL_COUNT_Y) : Canvas(CELL_COUNT_X, CELL_COUNT_Y, LayerSystem(CELL_COUNT_X, CELL_COUNT_Y, true)) { }

Canvas::Canvas(const int CELL_COUNT, const LayerSystem& layerSystem) : Canvas(CELL_COUNT, CELL_COUNT, layerSystem) { }
Canvas::Canvas(const int CELL_COUNT) : Canvas(CELL_COUNT, CELL_COUNT, LayerSystem(CELL_COUNT, CELL_COUNT, true)) { }

Canvas::Canvas(const LayerSystem& layerSystem) : Canvas(32, 32, layerSystem) { }
Canvas::Canvas() : Canvas(32, 32, LayerSystem(32, 32, true)) { }

void Canvas::Unload() {
    layerSystem.Unload();
    UnloadTexture(m_CanvasBackground);

    TraceLog(LOG_INFO, "CANVAS: Canvas unloaded successfully.");
}

void Canvas::Update(Camera2D& camera, Vector2 viewportPosition) {
    cursor.UpdatePreviousFramePosition();

    if(m_ReloadLayerTexture) {
        layerSystem.UpdateLayer();
        m_ReloadLayerTexture = false;
    }

    if(m_ReloadCanvas) {
        for(int i = 0; i < layerSystem.GetCount(); i++) {
            layerSystem.UpdateLayer(i);
        }

        m_ReloadCanvas = false;
    }
}

void Canvas::Render(Camera2D& camera, Vector2 viewportPosition, ColorSystem& colorSystem) {    
    DrawBackground();

    for(int i = layerSystem.GetCount() - 1; i >= 0; i--) {
        DrawLayer(layerSystem.GetLayer(i).IsVisible(), layerSystem.GetLayer(i));
    }

    // DrawCanvasGrid();
    DrawCanvasCursor(camera, viewportPosition, colorSystem);
    DrawCanvasFrame();
}

void Canvas::ToggleLayerReload() {
    m_ReloadLayerTexture = true;
    m_ReloadCanvas = false;
}

void Canvas::ToggleCanvasReload() {
    m_ReloadCanvas = true;
    m_ReloadLayerTexture = false;
}


Vector2 Canvas::GetCanvasSize(const int COUNT_X, const int COUNT_Y) {
    const float DEFAULT_SIZE = 16.0f;
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

    TraceLog(LOG_INFO, TextFormat("CANVAS: Canvas size: %0.1f / %0.1f (%ix%i)", result.x * COUNT_X, result.y * COUNT_Y, COUNT_X, COUNT_Y));

    return { result.x * COUNT_X, result.y * COUNT_Y};
}

Vector2 Canvas::GetCellSize() {
    return {
        SIZE.x * scale / CELL_COUNT_X, 
        SIZE.y * scale / CELL_COUNT_Y 
    };
}

Vector2 Canvas::PositionInWorldSpace(Vector2 screenspacePosition, Vector2 viewportPosition, Camera2D& camera) {
    return {
        GetScreenToWorld2D({ screenspacePosition.x - viewportPosition.x, screenspacePosition.y - viewportPosition.y }, camera).x,
        GetScreenToWorld2D({ screenspacePosition.x - viewportPosition.x, screenspacePosition.y - viewportPosition.y }, camera).y
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

void Canvas::DrawCell(float x, float y, Color color) {
    DrawRectangle(
        x,
        y,
        GetCellSize().x,
        GetCellSize().y,
        color
    );
}

void Canvas::DrawCellLines(float x, float y, float thickness, Color color) {
    DrawRectangleLinesEx(
        (Rectangle) { 
            x,
            y,
            GetCellSize().x,
            GetCellSize().y
        }, 
        thickness, 
        color
    );
}


void Canvas::DrawCanvasGrid() {
    for(int y = 0; y < CELL_COUNT_Y; y++) {
        for(int x = 0; x < CELL_COUNT_X; x++) {
            DrawCellLines(
                x * GetCellSize().x,
                y * GetCellSize().y,
                1.0f / scale, 
                Fade(BLACK, 0.5f)
            );
        }
    }        
}

void Canvas::DrawCanvasCursor(Camera2D& camera, Vector2 viewportPosition, ColorSystem& colorSystem) {
    Vector2 positionAsCanvasIndex = PositionAsCanvasIndex(PositionInWorldSpace(GetMousePosition(), viewportPosition, camera));

    if((positionAsCanvasIndex.x >= 0 && positionAsCanvasIndex.x < CELL_COUNT_X) && (positionAsCanvasIndex.y >= 0 && positionAsCanvasIndex.y < CELL_COUNT_Y)) {
        DrawCell(
            static_cast<int>(PositionAsCanvasCell(PositionInWorldSpace(GetMousePosition(), viewportPosition, camera)).x * scale),
            static_cast<int>(PositionAsCanvasCell(PositionInWorldSpace(GetMousePosition(), viewportPosition, camera)).y * scale),
            colorSystem.GetColor()
        );
    } else {
        DrawCellLines(
            static_cast<int>(PositionAsCanvasCell(PositionInWorldSpace(GetMousePosition(), viewportPosition, camera)).x * scale),
            static_cast<int>(PositionAsCanvasCell(PositionInWorldSpace(GetMousePosition(), viewportPosition, camera)).y * scale),
            2.0f / scale, 
            colorSystem.GetColor()
        );
    }

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