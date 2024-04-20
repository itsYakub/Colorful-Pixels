#include "Canvas.hpp"
#include "Viewport.hpp"

Canvas::Canvas(Viewport* viewport, const int CELL_COUNT_X, const int CELL_COUNT_Y) : 
    m_Viewport(viewport),
    m_Camera( (Camera2D) { 0 }),
    m_CameraOffset( (const Vector2) { viewport->GetSize().x / 2.0f, viewport->GetSize().y / 2.0f } ),

    m_PreviousFrameMousePosition(MouseWorldPosition()),
    m_Position( (Vector2) { 0.0f, 0.0f } ), 
    SIZE(GetCanvasSize(CELL_COUNT_X, CELL_COUNT_Y)),

    CELL_COUNT_X(CELL_COUNT_X), 
    CELL_COUNT_Y(CELL_COUNT_Y),

    m_Scale(1.0f),

    m_LayerList(0),
    m_CurrentLayerID(0),
    
    m_ColorPalette(),
    m_CurrentColor(RAYWHITE),
    
    m_CurrentTool(std::make_unique<Brush>(*this)) {
        m_LayerList.push_back(std::make_unique<Layer>(CELL_COUNT_X, CELL_COUNT_Y, 0, true, false));

        m_Camera.target = m_Position;
        m_Camera.offset = m_CameraOffset;
        m_Camera.zoom = m_Scale;
}

Canvas::Canvas(Viewport* viewport) : Canvas(viewport, 32, 32) { }
Canvas::Canvas(Viewport* viewport, const int CELL_COUNT) : Canvas(viewport, CELL_COUNT, CELL_COUNT) { }

void Canvas::Unload() {
    for(auto& i : m_LayerList) {
        i->Unload();
    }
}

void Canvas::Update() {
    // Check if mouse cursor is placed on the viewport...
    if(MouseViewportHover()) {
        // ...Check if mouse cursor is placed on the canvas...
        if(IsMouseCanvasIndexValid(MouseWorldPositionScaled()) && IsMouseCanvasIndexValid(PreviousFrameMousePositionScaled()) && !m_LayerList.at(m_CurrentLayerID)->IsLocked()) {
            // ...Check if left mouse button is pressed
            if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                m_CurrentTool->OnButtonDown();
            }
        }

        // ...Check if scroll is pressed
        if(IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
            Vector2 delta = GetMouseDelta();
            delta = Vector2Scale(delta, -1.0f / m_Camera.zoom);

            m_Camera.target = Vector2Add(m_Camera.target, delta);
        }

        // ...Check if scroll accelerates
        if(GetMouseWheelMove() != 0.0f) {
            m_Scale += GetMouseWheelMove() / 10.0f;
            m_Scale = Clamp(m_Scale, 0.5f, 4.0f);
            
            m_Camera.target = GetScreenToWorld2D(GetMousePosition(), m_Camera);
            m_Camera.offset = GetMousePosition();
            m_Camera.zoom = m_Scale;
        }
    }

    m_LayerList.at(m_CurrentLayerID)->UpdateLayer();
    m_PreviousFrameMousePosition = MouseWorldPosition();
}

void Canvas::Render() {    
    BeginMode2D(m_Camera);

        for(int i = m_LayerList.size() - 1; i >= 0; i--) {
            DrawLayer(m_LayerList.at(i)->IsVisible(), *m_LayerList.at(i));
        }

        DrawCanvasCursor(true);
        DrawCanvasGrid(true, CELL_COUNT_X, CELL_COUNT_Y);
        DrawCanvasFrame(true);

    EndMode2D();
}

void Canvas::ColorGuiPanel(const char* name, ImVec2 position, ImVec2 size) {
    ImGui::Begin(
        name, 
        NULL, 
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration
    );

    ImGui::SetWindowPos(position);
    ImGui::SetWindowSize(size);

    float colorArray[4] = { m_CurrentColor.r / 255.0f, m_CurrentColor.g / 255.0f, m_CurrentColor.b / 255.0f, m_CurrentColor.a / 255.0f };

    ImGui::SeparatorText("Color Picker");

    ImGui::PushItemWidth(size.x - 16.0f);
    ImGui::ColorPicker4(
        "Color Picker", 
        colorArray, 
        ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoLabel
    );

    m_CurrentColor.r = colorArray[0] * 255;
    m_CurrentColor.g = colorArray[1] * 255;
    m_CurrentColor.b = colorArray[2] * 255;
    m_CurrentColor.a = colorArray[3] * 255;

    ImGui::SeparatorText("Color Palette");

    ImGui::BeginChild(1);

    for(int i = 0, colorsInARow = 4; i < m_ColorPalette.Size(); i += colorsInARow) {
        ImGui::NewLine();

        for(int j = 0; j < colorsInARow; j++) {
            if(i + j >= m_ColorPalette.Size()) {
                break;
            }

            Color colorPaletteColor = m_ColorPalette.GetColor(i + j);

            ImGui::SameLine();
            ImGui::PushID(i + j);
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(colorPaletteColor.r / 255.0f, colorPaletteColor.g / 255.0f, colorPaletteColor.b / 255.0f, 1.0f));
            if(ImGui::Button("##color_button", ImVec2(32, 32))) {
                m_CurrentColor = colorPaletteColor;
            }
            ImGui::PopStyleColor();
            ImGui::PopID();
        }
    }

    ImGui::EndChild();

    ImGui::End();

}

void Canvas::LayersGuiPanel(const char* name, ImVec2 position, ImVec2 size) {
    ImGui::Begin(
        name, 
        NULL, 
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse
    );

    ImGui::SetWindowPos(position);
    ImGui::SetWindowSize(size);

    if(ImGui::Button("Add layer")) {
        m_LayerList.push_back(std::make_unique<Layer>(CELL_COUNT_X, CELL_COUNT_Y, m_LayerList.size(), true, false));
        m_CurrentLayerID++;
    }

    ImGui::SameLine();

    if(ImGui::Button("Remove layer") && m_LayerList.size() > 1) {
        m_LayerList.erase(m_LayerList.begin() + m_CurrentLayerID);

        if(m_CurrentLayerID < 0) {
            m_CurrentLayerID++;
        } else if(m_CurrentLayerID >= m_LayerList.size()) {
            m_CurrentLayerID--;
        }
    }

    ImGui::Separator();

    ImGui::BeginChild(1);

    for(int i = 0; i < m_LayerList.size(); i++) {
        if(ImGui::Button(m_CurrentLayerID == i ? TextFormat("Layer no.%i (Current)", m_LayerList.at(i)->GetID()) : TextFormat("Layer no.%i", m_LayerList.at(i)->GetID()), ImVec2(256.0f, 20.0f))) {
            m_CurrentLayerID = i;
        }

        ImGui::SameLine();
        ImGui::PushID(i);
            ImGui::Checkbox("##visible", &m_LayerList.at(i)->layerVisible);
        ImGui::PopID();

        ImGui::SameLine();
        ImGui::PushID(i);
            ImGui::Checkbox("##locked", &m_LayerList.at(i)->layerLocked);
        ImGui::PopID();

        ImGui::SameLine();
        ImGui::PushID(i);

            if(ImGui::Button("Up")) {
                if(i - 1 >= 0) {
                    std::swap(m_LayerList.at(i), m_LayerList.at(i - 1));
                }
            }

        ImGui::PopID();

        ImGui::SameLine();
        ImGui::PushID(i);
        
            if(ImGui::Button("Down")) {
                if(i + 1 < m_LayerList.size()) {
                    std::swap(m_LayerList.at(i), m_LayerList.at(i + 1));
                }
            }

        ImGui::PopID();
    }

    ImGui::EndChild();
    ImGui::End();    
}

void Canvas::ToolsGuiPanel(const char* name, ImVec2 position, ImVec2 size) {
    ImGui::Begin(
        name, 
        NULL, 
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration
    );

    ImGui::SetWindowPos(position);
    ImGui::SetWindowSize(size);

    if(ImGui::Button("##Brush", ImVec2(size.x - 16, size.x - 16))) {
        m_CurrentTool.release();
        m_CurrentTool = std::make_unique<Brush>(*this);

        TraceLog(LOG_INFO, "Current tool: BRUSH");
    }

    if(ImGui::Button("##Eraser", ImVec2(size.x - 16, size.x - 16))) {
        m_CurrentTool.release();
        m_CurrentTool = std::make_unique<Eraser>(*this);

        TraceLog(LOG_INFO, "Current tool: ERASER");
    }

    ImGui::End(); 
}

std::unique_ptr<Layer>& Canvas::GetLayer() {
    return m_LayerList.at(m_CurrentLayerID);
}

std::unique_ptr<Layer>& Canvas::GetLayer(int index) {
    return m_LayerList.at(index);
}

Color Canvas::GetColor() {
    return m_CurrentColor;
}

void Canvas::SetColor(Color color) {
    m_CurrentColor = color;
}

// `GetMouseCanvasIndex()` - This function returns the cell index of the canvas that the cursor is currently hovering above
Vector2 Canvas::GetMouseCanvasIndex(Vector2 mousePositionScaled) {
    return { 
        floor(mousePositionScaled.x / SIZE.x * CELL_COUNT_X),
        floor(mousePositionScaled.y / SIZE.y * CELL_COUNT_Y)
    };
}

// `GetMouseWorldPosition()` - This function returns the position of the mouse cursor in the 2D World space ( `GetScreenToWorld2D()` )
Vector2 Canvas::MouseWorldPosition() {
    return {
        GetScreenToWorld2D(GetMousePosition(), m_Camera).x + GetCanvasOriginOffset().x - m_Viewport->GetPosition().x / m_Scale,
        GetScreenToWorld2D(GetMousePosition(), m_Camera).y + GetCanvasOriginOffset().y - m_Viewport->GetPosition().y / m_Scale 
    };
}

// `GetMouseWorldPositionScaled()` - This function returns the position of the mouse cursor in the 2D World space divided by the scale factor ( `scale` )
Vector2 Canvas::MouseWorldPositionScaled() {
    return {
        MouseWorldPosition().x / m_Scale,
        MouseWorldPosition().y / m_Scale
    };
}

Vector2& Canvas::PreviousFrameMousePosition() {
    return m_PreviousFrameMousePosition;
}

Vector2 Canvas::PreviousFrameMousePositionScaled() {
    return {
        m_PreviousFrameMousePosition.x / m_Scale,
        m_PreviousFrameMousePosition.y / m_Scale
    };
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
        m_LayerList.at(m_CurrentLayerID)->SetPixelColor(std::floor(x), std::floor(y), color);
        x += dx;
        y += dy;
        i++;
    }
}

// `GetCanvasSize()` - This function returns the size of the canvas based on the count of the cells in X and Y axis (default resolution is 512px)
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

// `GetCanvasOriginOffset()` - This function returns the origin offset from the center of the canvas
Vector2 Canvas::GetCanvasOriginOffset() {
    return {
        SIZE.x * m_Scale / 2.0f,
        SIZE.y * m_Scale / 2.0f
    };
}

// `GetMouseWorldCanvasPosition()` - This function returns the position of the cell in the canvas, based on the current mouse position in the 2D World ( `GetMouseWorldPosition()` )
Vector2 Canvas::GetMouseToCanvasCellPosition(Vector2 mouseCanvasIndex) {
    return {
        m_Position.x - GetCanvasOriginOffset().x + mouseCanvasIndex.x * (SIZE.x * m_Scale / CELL_COUNT_X),
        m_Position.y - GetCanvasOriginOffset().y + mouseCanvasIndex.y * (SIZE.y * m_Scale / CELL_COUNT_Y)
    };
}

// `IsMouseCanvasIndexValid()` - This function check's if `GetMouseCanvasIndex()` returns a valid index for the `positionScaled` mouse position
bool Canvas::IsMouseCanvasIndexValid(Vector2 positionScaled) {
    return (GetMouseCanvasIndex(positionScaled).x >= 0 && GetMouseCanvasIndex(positionScaled).x < CELL_COUNT_X) &&
        (GetMouseCanvasIndex(positionScaled).y >= 0 && GetMouseCanvasIndex(positionScaled).y < CELL_COUNT_Y) &&
        MouseViewportHover();
}

bool Canvas::MouseViewportHover() {
    return CheckCollisionPointRec(
        GetMousePosition(), 
        (Rectangle) {
            m_Viewport->GetPosition().x,
            m_Viewport->GetPosition().y,
            m_Viewport->GetSize().x,
            m_Viewport->GetSize().y
        }
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
        GetCanvasOriginOffset(),
        0.0f,
        WHITE
    );    
}

void Canvas::DrawCanvasGrid(bool visible, const int WIDTH, const int HEIGHT) {
    if(!visible) {
        return;
    }

    for(int y = 0; y < HEIGHT; y++) {
        for(int x = 0; x < WIDTH; x++) {
            DrawRectangleLinesEx(
                (Rectangle) { 
                    m_Position.x - GetCanvasOriginOffset().x + (x * (SIZE.x * m_Scale / WIDTH)),
                    m_Position.y - GetCanvasOriginOffset().y + (y * (SIZE.y * m_Scale / HEIGHT)),
                    SIZE.x * m_Scale / WIDTH, 
                    SIZE.y * m_Scale / HEIGHT 
                }, 
                0.5f / m_Scale, 
                Fade(LIGHTGRAY, 0.5f)
            );
        }
    }        
}

void Canvas::DrawCanvasCursor(bool visible) {
    if(!visible && MouseViewportHover()) {
        return;
    }

    DrawRectangleLinesEx(
        (Rectangle) {
            GetMouseToCanvasCellPosition(GetMouseCanvasIndex(MouseWorldPositionScaled())).x,
            GetMouseToCanvasCellPosition(GetMouseCanvasIndex(MouseWorldPositionScaled())).y,
            SIZE.x * m_Scale / CELL_COUNT_X, 
            SIZE.y * m_Scale / CELL_COUNT_Y 
        },
        2.0f / m_Scale,
        WHITE
    );
}

void Canvas::DrawCanvasFrame(bool visible) {
    if(!visible) {
        return;
    }

    DrawRectangleLinesEx(
        (Rectangle) {
            m_Position.x - GetCanvasOriginOffset().x,
            m_Position.y - GetCanvasOriginOffset().y,
            SIZE.x * m_Scale,
            SIZE.y * m_Scale
        },
        2.0f / m_Scale,
        BLACK
    );
}
