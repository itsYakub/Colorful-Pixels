#include <string>
#include <cmath>
#include <memory>
#include <vector>

#include "raylib.h"
#include "raymath.h"

#include "imgui.h"
#include "rlImGui.h"

#ifdef PLATFORM_WEB
    #include "emscripten.h"
#endif

class Viewport {
private:
    RenderTexture2D m_RenderTexture;

    Vector2 m_Position;
    Vector2 m_Size;

public:
    Viewport(const Vector2 position, const Vector2 size) :
        m_RenderTexture(LoadRenderTexture(size.x, size.y)),
        m_Position(position), m_Size(size) { }

    void Unload() {
        UnloadRenderTexture(m_RenderTexture);
    }

   void ViewportGuiPanel(const char* name, ImVec2 position, ImVec2 size) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin(
            name, 
            NULL, 
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration
        );

        ImGui::SetWindowPos(position);
        ImGui::SetWindowSize(size);

        rlImGuiImageRenderTextureFit(&m_RenderTexture, true);

        ImGui::PopStyleVar(1);

        ImGui::End();
   }

    void Begin() {
        BeginTextureMode(m_RenderTexture);
    }

    void Clear(Color color) {
        ClearBackground(color);
    }

    void End() {
        EndTextureMode();
    }

    Vector2 GetPosition() {
        return m_Position;
    }

    Vector2 GetSize() {
        return m_Size;
    }
};

class ColorPalette {
private:
    std::vector<Color> m_ColorList;

public:
    ColorPalette() : m_ColorList() {
        m_ColorList.push_back(LIGHTGRAY);
        m_ColorList.push_back(GRAY);
        m_ColorList.push_back(DARKGRAY);
        m_ColorList.push_back(YELLOW);
        m_ColorList.push_back(GOLD);
        m_ColorList.push_back(ORANGE);
        m_ColorList.push_back(PINK);
        m_ColorList.push_back(RED);
        m_ColorList.push_back(MAROON);
        m_ColorList.push_back(GREEN);
        m_ColorList.push_back(LIME);
        m_ColorList.push_back(DARKGREEN);
        m_ColorList.push_back(SKYBLUE);
        m_ColorList.push_back(BLUE);
        m_ColorList.push_back(DARKBLUE);
        m_ColorList.push_back(PURPLE);
        m_ColorList.push_back(VIOLET);
        m_ColorList.push_back(DARKPURPLE);
        m_ColorList.push_back(BEIGE);
        m_ColorList.push_back(BROWN);
        m_ColorList.push_back(DARKBROWN);
        m_ColorList.push_back(WHITE);
        m_ColorList.push_back(BLACK);
        m_ColorList.push_back(MAGENTA);
        m_ColorList.push_back(RAYWHITE);
    }

    int Size() {
        return m_ColorList.size();
    }

    Color GetColor(int index) {
        return m_ColorList.at(Clamp(index, 0, m_ColorList.size() - 1));
    }
};

class Layer {
private:
    const Vector2 COUNT;
    const int ID;

    std::vector<Color> m_LayerData;
    Texture2D m_LayerTexture;

public:
    bool layerVisible;
    bool layerLocked;
    
public:
    Layer(const int CELL_COUNT_X, const int CELL_COUNT_Y, int ID, bool visibility, bool lock) : 
        COUNT((const Vector2) { static_cast<float>(CELL_COUNT_X), static_cast<float>(CELL_COUNT_Y) } ), 
        ID(ID),
        m_LayerData(CELL_COUNT_X * CELL_COUNT_Y),
        layerVisible(visibility),
        layerLocked(lock) {
            Load();
    }

    Layer(const int CELL_COUNT, int ID) : Layer(CELL_COUNT, CELL_COUNT, ID, true, false) { }
    Layer(const int CELL_COUNT, int ID, bool visibility) : Layer(CELL_COUNT, CELL_COUNT, ID, visibility, false) { }
    Layer(const int CELL_COUNT, int ID, bool visibility, bool lock) : Layer(CELL_COUNT, CELL_COUNT, ID, visibility, lock) { }
    Layer(const int CELL_COUNT_X, const int CELL_COUNT_Y, int ID) : Layer(CELL_COUNT_X, CELL_COUNT_Y, ID, true, false) { }
    Layer(const int CELL_COUNT_X, const int CELL_COUNT_Y, int ID, bool visibility) : Layer(CELL_COUNT_X, CELL_COUNT_Y, ID, visibility, false) { }

    void Load() {
        for(int i = 0; i < COUNT.x * COUNT.y; i++) {
            m_LayerData[i] = BLANK;
        }  

        Image image = GenImageColor(COUNT.x, COUNT.y, BLANK);
        m_LayerTexture = LoadTextureFromImage(image);
        SetTextureFilter(m_LayerTexture, TEXTURE_FILTER_POINT);
        UnloadImage(image);  
    }

    void Unload() {
        UnloadTexture(m_LayerTexture);
    }

    int GetID() {
        return ID;
    }

    Texture2D& GetTexture() {
        return m_LayerTexture;
    }

    std::vector<Color>& GetData() {
        return m_LayerData;
    }

    bool IsVisible() {
        return layerVisible;
    }

    void SetVisibility(bool visibility) {
        layerVisible = visibility;
    }

    bool IsLocked() {
        return layerLocked;
    }

    void SetLock(bool lock) {
        layerLocked = lock;
    }

    void SetPixelColor(int x, int y, Color color) {
        x = Clamp(x, 0, COUNT.x - 1);
        y = Clamp(y, 0, COUNT.y - 1);

        m_LayerData.at(y * COUNT.x + x) = color;
    }

    void UpdateLayer() {
        UpdateTexture(m_LayerTexture, m_LayerData.data());
    }
};

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


public:
    Canvas(Viewport* viewport, const int CELL_COUNT_X, const int CELL_COUNT_Y) : 
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
        m_CurrentColor(RAYWHITE) {
            m_LayerList.push_back(std::make_unique<Layer>(CELL_COUNT_X, CELL_COUNT_Y, 0, true, false));

            m_Camera.target = m_Position;
            m_Camera.offset = m_CameraOffset;
            m_Camera.zoom = m_Scale;
    }

    Canvas(Viewport* viewport) : Canvas(viewport, 32, 32) { }
    Canvas(Viewport* viewport, const int CELL_COUNT) : Canvas(viewport, CELL_COUNT, CELL_COUNT) { }

    void Unload() {
        for(auto& i : m_LayerList) {
            i->Unload();
        }
    }

    void Update() {
        // Check if mouse cursor is placed on the viewport...
        if(MouseViewportHover()) {
            // ...Check if mouse cursor is placed on the canvas...
            if(IsMouseCanvasIndexValid(MouseWorldPositionScaled()) && IsMouseCanvasIndexValid(PreviousFrameMousePositionScaled()) && !m_LayerList.at(m_CurrentLayerID)->IsLocked()) {
                int ax = GetMouseCanvasIndex(MouseWorldPositionScaled()).x;
                int ay = GetMouseCanvasIndex(MouseWorldPositionScaled()).y;

                int bx = GetMouseCanvasIndex(PreviousFrameMousePositionScaled()).x;
                int by = GetMouseCanvasIndex(PreviousFrameMousePositionScaled()).y;

                // ...Check if left mouse button is pressed
                if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                    DigitalDifferentialAnalyzer(ax, ay, bx, by, m_CurrentColor);
                }

                // ...Check if right mouse button is pressed
                if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                    DigitalDifferentialAnalyzer(ax, ay, bx, by, BLANK);
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

    void Render() {    
        BeginMode2D(m_Camera);

            for(int i = m_LayerList.size() - 1; i >= 0; i--) {
                DrawLayer(m_LayerList.at(i)->IsVisible(), *m_LayerList.at(i));
            }

            DrawCanvasCursor(true);
            DrawCanvasGrid(true, CELL_COUNT_X, CELL_COUNT_Y);
            DrawCanvasFrame(true);

        EndMode2D();
    }

    void ColorGuiPanel(const char* name, ImVec2 position, ImVec2 size) {
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

    void LayersGuiPanel(const char* name, ImVec2 position, ImVec2 size) {
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

    void ToolsGuiPanel(const char* name, ImVec2 position, ImVec2 size) {
        ImGui::Begin(
            name, 
            NULL, 
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration
        );

        ImGui::SetWindowPos(position);
        ImGui::SetWindowSize(size);

        if(ImGui::Button("##Brush", ImVec2(size.x - 16, size.x - 16))) {
            TraceLog(LOG_INFO, "Brush button!");
        }

        if(ImGui::Button("##Eraser", ImVec2(size.x - 16, size.x - 16))) {
            TraceLog(LOG_INFO, "Eraser button!");
        }

        ImGui::End(); 
    }

    std::unique_ptr<Layer>& GetLayer() {
        return m_LayerList.at(m_CurrentLayerID);
    }

    std::unique_ptr<Layer>& GetLayer(int index) {
        return m_LayerList.at(index);
    }

    Color GetColor() {
        return m_CurrentColor;
    }

    void SetColor(Color color) {
        m_CurrentColor = color;
    }

    // `GetMouseCanvasIndex()` - This function returns the cell index of the canvas that the cursor is currently hovering above
    Vector2 GetMouseCanvasIndex(Vector2 mousePositionScaled) {
        return { 
            floor(mousePositionScaled.x / SIZE.x * CELL_COUNT_X),
            floor(mousePositionScaled.y / SIZE.y * CELL_COUNT_Y)
        };
    }

    // `GetMouseWorldPosition()` - This function returns the position of the mouse cursor in the 2D World space ( `GetScreenToWorld2D()` )
    Vector2 MouseWorldPosition() {
        return {
            GetScreenToWorld2D(GetMousePosition(), m_Camera).x + GetCanvasOriginOffset().x - m_Viewport->GetPosition().x / m_Scale,
            GetScreenToWorld2D(GetMousePosition(), m_Camera).y + GetCanvasOriginOffset().y - m_Viewport->GetPosition().y / m_Scale 
        };
    }

    // `GetMouseWorldPositionScaled()` - This function returns the position of the mouse cursor in the 2D World space divided by the scale factor ( `scale` )
    Vector2 MouseWorldPositionScaled() {
        return {
            MouseWorldPosition().x / m_Scale,
            MouseWorldPosition().y / m_Scale
        };
    }

    Vector2& PreviousFrameMousePosition() {
        return m_PreviousFrameMousePosition;
    }

    Vector2 PreviousFrameMousePositionScaled() {
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
    void DigitalDifferentialAnalyzer(int ax, int ay, int bx, int by, Color color) {
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

private:
    // `GetCanvasSize()` - This function returns the size of the canvas based on the count of the cells in X and Y axis (default resolution is 512px)
    Vector2 GetCanvasSize(const int COUNT_X, const int COUNT_Y) {
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
    Vector2 GetCanvasOriginOffset() {
        return {
            SIZE.x * m_Scale / 2.0f,
            SIZE.y * m_Scale / 2.0f
        };
    }

    // `GetMouseWorldCanvasPosition()` - This function returns the position of the cell in the canvas, based on the current mouse position in the 2D World ( `GetMouseWorldPosition()` )
    Vector2 GetMouseToCanvasCellPosition(Vector2 mouseCanvasIndex) {
        return {
            m_Position.x - GetCanvasOriginOffset().x + mouseCanvasIndex.x * (SIZE.x * m_Scale / CELL_COUNT_X),
            m_Position.y - GetCanvasOriginOffset().y + mouseCanvasIndex.y * (SIZE.y * m_Scale / CELL_COUNT_Y)
        };
    }

    // `IsMouseCanvasIndexValid()` - This function check's if `GetMouseCanvasIndex()` returns a valid index for the `positionScaled` mouse position
    bool IsMouseCanvasIndexValid(Vector2 positionScaled) {
        return (GetMouseCanvasIndex(positionScaled).x >= 0 && GetMouseCanvasIndex(positionScaled).x < CELL_COUNT_X) &&
            (GetMouseCanvasIndex(positionScaled).y >= 0 && GetMouseCanvasIndex(positionScaled).y < CELL_COUNT_Y) &&
            MouseViewportHover();
    }

    bool MouseViewportHover() {
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

    void DrawLayer(bool visible, Layer& layer) {
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

    void DrawCanvasGrid(bool visible, const int WIDTH, const int HEIGHT) {
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

    void DrawCanvasCursor(bool visible) {
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

    void DrawCanvasFrame(bool visible) {
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
};

// TODO(yakub):
// I think it's time to finally separate all the classes to their respective header and source files.
// To go further we need file inclusion, we need more flexibility then what we have right now. 

class Tool {
protected:
    Canvas* m_Canvas;

public:
    Tool(Canvas* canvas) : m_Canvas(canvas) { }

    virtual void OnButtonPress();
    virtual void OnButtonDown();
    virtual void OnButtonRelease();
};

class Brush : protected Tool {
public:
    Brush(Canvas* canvas) : Tool(canvas) { }

    void OnButtonDown() override {
        int ax = m_Canvas->GetMouseCanvasIndex(m_Canvas->MouseWorldPositionScaled()).x;
        int ay = m_Canvas->GetMouseCanvasIndex(m_Canvas->MouseWorldPositionScaled()).y;

        int bx = m_Canvas->GetMouseCanvasIndex(m_Canvas->PreviousFrameMousePositionScaled()).x;
        int by = m_Canvas->GetMouseCanvasIndex(m_Canvas->PreviousFrameMousePositionScaled()).y;

        m_Canvas->DigitalDifferentialAnalyzer(ax, ay, bx, by, m_Canvas->GetColor());
    }
};

class Eraser : protected Tool {
public:
    Eraser(Canvas* canvas) : Tool(canvas) { }

    void OnButtonDown() override {
        int ax = m_Canvas->GetMouseCanvasIndex(m_Canvas->MouseWorldPositionScaled()).x;
        int ay = m_Canvas->GetMouseCanvasIndex(m_Canvas->MouseWorldPositionScaled()).y;

        int bx = m_Canvas->GetMouseCanvasIndex(m_Canvas->PreviousFrameMousePositionScaled()).x;
        int by = m_Canvas->GetMouseCanvasIndex(m_Canvas->PreviousFrameMousePositionScaled()).y;

        m_Canvas->DigitalDifferentialAnalyzer(ax, ay, bx, by, BLANK);
    }
};

class ColorfulPixels {
private:
    std::unique_ptr<Viewport> m_Viewport;
    std::unique_ptr<Canvas> m_Canvas;

public:
    ColorfulPixels() { }

    void Load() {
        m_Viewport = std::make_unique<Viewport>(
            (const Vector2) { 
                192, 
                0 
            }, (const Vector2) { 
                768, 
                512 
            }
        );

        m_Canvas = std::make_unique<Canvas>(m_Viewport.get(), 16);

        rlImGuiSetup(true);
    }

    void Unload() {
        rlImGuiShutdown();

        m_Viewport->Unload();
        m_Canvas->Unload();
    }

    void Update() {
        m_Canvas->Update();
    }

    void Render() {
        m_Viewport->Begin();
        m_Viewport->Clear(DARKBLUE);

            m_Canvas->Render();

        m_Viewport->End();
    }

    void RenderGUI() {
        ClearBackground(BLACK);

        rlImGuiBegin();

            m_Viewport->ViewportGuiPanel("Panel: Viewport", ImVec2(192.0f, 0.0f), ImVec2(768.0f, 512.0f));

            m_Canvas->ColorGuiPanel("Panel: Colors", ImVec2(0.0f, 0.0f), ImVec2(192.0f, 512.0f));
            m_Canvas->LayersGuiPanel("Panel: Layers", ImVec2(0.0f, 512.0f), ImVec2(1024.0f, 256.0f));
            m_Canvas->ToolsGuiPanel("Panel: Tools", ImVec2(960.0f, 0.0f), ImVec2(64.0f, 512.0f));

        rlImGuiEnd();    
    }
};

// Function called on every frame. Handles the basic updating and rendering of the current game frame.
// (NOTE: c-style function is necessary for the emscritpen; it won't accept c++ methods)
// - void* args - the `Game` class instance.
void UpdateRenderFrame(void* args);

// The main game class
class Game {
private:
    const int WIDTH; // Window width.
    const int HEIGHT; // Window width
    const std::string TITLE; // Window title.

    ColorfulPixels colorfulPixels;

public:
    // `Game` class constructor.
    Game() : WIDTH(1024), HEIGHT(768), TITLE(TextFormat("Raylib %s - Colorful Pixels %s", RAYLIB_VERSION, "1.0")), colorfulPixels() {
        // Initializing the Audio Device and creating a Window.
        InitAudioDevice();
        InitWindow(WIDTH, HEIGHT, TITLE.c_str());

        colorfulPixels.Load();

#ifdef PLATFORM_WEB
        // Passing the `UpdateRenderFrame` function with the argument `this` for this `Game` class instance.
        // Value 0 caps the game's framerate to the default browser's framerate.
        // Value 1 tells emscritpen to simulate the infinite loop.
        emscripten_set_main_loop_arg(UpdateRenderFrame, this, 0, 1);
#else   
        // Caping the FPS to 60.
        SetTargetFPS(60);
        // You can also use the `SetConfigFlag(FLAG_VSYNC_HINT)` before creating the window to cap the framerate to your monitor's refresh rate.
        while(!WindowShouldClose()) {
            UpdateRenderFrame(this);
        }
#endif

    }

    // `Game` class destructor.
    ~Game() {
        // Deinitializing the game's resources.
        colorfulPixels.Unload();

        CloseAudioDevice();
        CloseWindow();
    }

    // This function is called on every game's cycle.
    // Purpose: update the internal components, logic, etc.
    void Update() {
        colorfulPixels.Update();
    }

    // This function is called on every game's cycle.
    // Purpose: render game's elements, components, text, etc.
    void Render() {
        colorfulPixels.Render();
        colorfulPixels.RenderGUI();
    }
};

// Function called on every frame. Handles the basic updating and rendering of the current game frame.
// (NOTE: c-style function is necessary for the emscritpen; it won't accept c++ methods)
// - viod* args - the `Game` class instance.
void UpdateRenderFrame(void* args) {
    static_cast<Game*>(args)->Update();

    BeginDrawing();
    static_cast<Game*>(args)->Render();
    EndDrawing();
}

int main(int, char**) {
    // Creating the game instance
    Game game;

    return 0;
}
