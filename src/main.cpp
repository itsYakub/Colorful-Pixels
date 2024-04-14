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

class Layer {
private:
    const Vector2 COUNT;

    std::vector<Color> m_LayerData;
    Texture2D m_LayerTexture;

public:
    bool layerVisible;
    bool layerLocked;
    
public:
    Layer(const int CELL_COUNT_X, const int CELL_COUNT_Y, bool visibility, bool lock) : 
        COUNT((const Vector2) { static_cast<float>(CELL_COUNT_X), static_cast<float>(CELL_COUNT_Y) } ), 
        m_LayerData(CELL_COUNT_X * CELL_COUNT_Y),
        layerVisible(visibility),
        layerLocked(lock) {
            Load();
    }

    Layer(const int CELL_COUNT) : Layer(CELL_COUNT, CELL_COUNT, true, false) { }
    Layer(const int CELL_COUNT, bool visibility) : Layer(CELL_COUNT, CELL_COUNT, visibility, false) { }
    Layer(const int CELL_COUNT, bool visibility, bool lock) : Layer(CELL_COUNT, CELL_COUNT, visibility, lock) { }
    Layer(const int CELL_COUNT_X, const int CELL_COUNT_Y) : Layer(CELL_COUNT_X, CELL_COUNT_Y, true, false) { }
    Layer(const int CELL_COUNT_X, const int CELL_COUNT_Y, bool visibility) : Layer(CELL_COUNT_X, CELL_COUNT_Y, visibility, false) { }

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
        m_LayerData.at(y * COUNT.x + x) = color;
    }

    void UpdateLayer() {
        UpdateTexture(m_LayerTexture, m_LayerData.data());
    }
};

class Canvas {
private:
    Camera2D m_Camera;
    const Vector2 m_CameraOffset;

    Vector2 m_Position;
    const Vector2 SIZE;

    const int CELL_COUNT_X;
    const int CELL_COUNT_Y;

    float m_Scale;

    std::vector<std::unique_ptr<Layer>> m_LayerList;
    int m_CurrentLayerID;

    Color m_CurrentColor;

    bool m_ColorPanelGUI;
    bool m_LayerPanelGUI;

    bool m_MouseOnGuiWidget;

public:
    Canvas(const int CELL_COUNT_X, const int CELL_COUNT_Y) : 
        m_Camera( (Camera2D) { 0 }),
        m_CameraOffset( (const Vector2) { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f } ),

        m_Position( (Vector2) { 0.0f, 0.0f } ), 
        SIZE(GetCanvasSize(CELL_COUNT_X, CELL_COUNT_Y)),

        CELL_COUNT_X(CELL_COUNT_X), 
        CELL_COUNT_Y(CELL_COUNT_Y),

        m_Scale(1.0f),

        m_LayerList(0),
        m_CurrentLayerID(0),
        
        m_CurrentColor(RAYWHITE),
        
        m_ColorPanelGUI(true),
        m_LayerPanelGUI(true),
        
        m_MouseOnGuiWidget(false) {
            m_LayerList.push_back(std::make_unique<Layer>(CELL_COUNT_X, CELL_COUNT_Y, true, false));

            m_Camera.target = m_Position;
            m_Camera.offset = m_CameraOffset;
            m_Camera.zoom = m_Scale;
    }

    Canvas() : Canvas(32, 32) { 

    }

    Canvas(const int CELL_COUNT) : Canvas(CELL_COUNT, CELL_COUNT) { 

    }

    void Unload() {
        for(auto& i : m_LayerList) {
            i->Unload();
        }
    }

    void Update() {
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !m_MouseOnGuiWidget) {
            if(IsMouseCanvasIndexValid() && !m_LayerList.at(m_CurrentLayerID)->IsLocked()) {
                m_LayerList.at(m_CurrentLayerID)->SetPixelColor(GetMouseCanvasIndex().x, GetMouseCanvasIndex().y, m_CurrentColor);
            }
        }

        if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && !m_MouseOnGuiWidget) {
            if(IsMouseCanvasIndexValid() && !m_LayerList.at(m_CurrentLayerID)->IsLocked()) {
                m_LayerList.at(m_CurrentLayerID)->SetPixelColor(GetMouseCanvasIndex().x, GetMouseCanvasIndex().y, BLANK);
            }
        }

        if(IsMouseButtonDown(MOUSE_BUTTON_MIDDLE) && !m_MouseOnGuiWidget) {
            Vector2 delta = GetMouseDelta();
			delta = Vector2Scale(delta, -1.0f / m_Camera.zoom);

			m_Camera.target = Vector2Add(m_Camera.target, delta);
        }

        if(GetMouseWheelMove() != 0.0f && !m_MouseOnGuiWidget) {
            m_Scale += GetMouseWheelMove() / 5.0f;
            m_Scale = Clamp(m_Scale, 0.5f, 2.0f);
			
            m_Camera.target = GetScreenToWorld2D(GetMousePosition(), m_Camera);
			m_Camera.offset = GetMousePosition();
            m_Camera.zoom = m_Scale;
        }

        m_LayerList.at(m_CurrentLayerID)->UpdateLayer();
    }

    void Render() {    
        BeginMode2D(m_Camera);

            for(auto& layer : m_LayerList) {
                DrawLayer(layer->IsVisible(), *layer);
            }

            DrawCanvasGrid(true, CELL_COUNT_X, CELL_COUNT_Y);
            DrawCanvasCursor(true);
            DrawCanvasFrame(true);

        EndMode2D();

        if(m_ColorPanelGUI) {
            ImGui::Begin(
                "Color", 
                NULL, 
                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse
            );

            ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
            ImGui::SetWindowSize(ImVec2(192.0f, 512.0f));

            m_MouseOnGuiWidget = ImGui::IsWindowHovered();

            float colorArray[4] = { m_CurrentColor.r / 255.0f, m_CurrentColor.g / 255.0f, m_CurrentColor.b / 255.0f, m_CurrentColor.a / 255.0f };

            ImGui::PushItemWidth(160.0f);
            ImGui::ColorPicker4(
                "Color Picker", 
                colorArray, 
                ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoLabel
            );


            m_CurrentColor.r = colorArray[0] * 255;
            m_CurrentColor.g = colorArray[1] * 255;
            m_CurrentColor.b = colorArray[2] * 255;
            m_CurrentColor.a = colorArray[3] * 255;

            ImGui::End();
        }

        if(m_LayerPanelGUI) {
            ImGui::Begin(
                "Layer", 
                NULL, 
                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse
            );

            ImGui::SetWindowPos(ImVec2(0.0f, GetScreenHeight() - 256.0f));
            ImGui::SetWindowSize(ImVec2(192.0f, 256.0f));


            m_MouseOnGuiWidget = ImGui::IsWindowHovered();

            if(ImGui::Button("Add layer")) {
                m_LayerList.push_back(std::make_unique<Layer>(CELL_COUNT_X, CELL_COUNT_Y, true, false));
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

            for(int i = 0; i < m_LayerList.size(); i++) {
                if(ImGui::Button(m_CurrentLayerID == i ? TextFormat("Cur. Layer") : TextFormat("Layer no.%i", i + 1))) {
                    m_CurrentLayerID = i;
                }

                ImGui::SameLine();
                ImGui::Checkbox("V.", &m_LayerList.at(i)->layerVisible);
                ImGui::SameLine();
                ImGui::Checkbox("L.", &m_LayerList.at(i)->layerLocked);
            }

            ImGui::End();
        }
    }

    std::unique_ptr<Layer>& GetLayer() {
        return m_LayerList.at(m_CurrentLayerID);
    }

    std::unique_ptr<Layer>& GetLayer(int index) {
        return m_LayerList.at(index);
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

    // `GetMouseWorldPosition()` - This function returns the position of the mouse cursor in the 2D World space ( `GetScreenToWorld2D()` )
    Vector2 GetMouseWorldPosition() {
        return {
            GetScreenToWorld2D(GetMousePosition(), m_Camera).x + GetCanvasOriginOffset().x,
            GetScreenToWorld2D(GetMousePosition(), m_Camera).y + GetCanvasOriginOffset().y
        };
    }

    // `GetMouseWorldPositionScaled()` - This function returns the position of the mouse cursor in the 2D World space divided by the scale factor ( `m_Scale` )
    Vector2 GetMouseWorldPositionScaled() {
        return {
            GetMouseWorldPosition().x / m_Scale,
            GetMouseWorldPosition().y / m_Scale
        };
    }

    // `GetMouseWorldCanvasPosition()` - This function returns the position of the cell in the canvas, based on the current mouse position in the 2D World ( `GetMouseWorldPosition()` )
    Vector2 GetMouseToCanvasCellPosition() {
        return {
            m_Position.x - GetCanvasOriginOffset().x + GetMouseCanvasIndex().x * (SIZE.x * m_Scale / CELL_COUNT_X),
            m_Position.y - GetCanvasOriginOffset().y + GetMouseCanvasIndex().y * (SIZE.y * m_Scale / CELL_COUNT_Y)
        };
    }

    // `GetMouseCanvasIndex()` - This function returns the cell index of the canvas that the cursor is currently hovering above
    Vector2 GetMouseCanvasIndex() {
        return { 
            floor(GetMouseWorldPositionScaled().x / SIZE.x * CELL_COUNT_X),
            floor(GetMouseWorldPositionScaled().y / SIZE.y * CELL_COUNT_Y)
        };
    }

    // `IsMouseCanvasIndexValid()` - This function check's if `GetMouseCanvasIndex()` returns a valid index
    bool IsMouseCanvasIndexValid() {
        return (GetMouseCanvasIndex().x >= 0 && GetMouseCanvasIndex().x < CELL_COUNT_X) &&
            (GetMouseCanvasIndex().y >= 0 && GetMouseCanvasIndex().y < CELL_COUNT_Y); 
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
        if(!visible || m_MouseOnGuiWidget) {
            return;
        }

        if(IsMouseCanvasIndexValid()) {
            DrawRectangleRec(
                (Rectangle) {
                    GetMouseToCanvasCellPosition().x,
                    GetMouseToCanvasCellPosition().y,
                    SIZE.x * m_Scale / CELL_COUNT_X, 
                    SIZE.y * m_Scale / CELL_COUNT_Y 
                },
                m_CurrentColor
            );
        } else {
            DrawRectangleLinesEx(
                (Rectangle) {
                    GetMouseToCanvasCellPosition().x,
                    GetMouseToCanvasCellPosition().y,
                    SIZE.x * m_Scale / CELL_COUNT_X, 
                    SIZE.y * m_Scale / CELL_COUNT_Y 
                },
                1.0f / m_Scale,
                WHITE
            );
        }
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

    std::unique_ptr<Canvas> canvas;

public:
    // `Game` class constructor.
    Game() : WIDTH(1024), HEIGHT(768), TITLE(TextFormat("Raylib %s - Colorful Pixels %s", RAYLIB_VERSION, "1.0")) {
        // Initializing the Audio Device and creating a Window.
        InitAudioDevice();
        InitWindow(WIDTH, HEIGHT, TITLE.c_str());

        rlImGuiSetup(true);

        canvas = std::make_unique<Canvas>(32);

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
        canvas->Unload();

        // Deinitializing the game's resources.
        rlImGuiShutdown();

        CloseAudioDevice();
        CloseWindow();
    }

    // This function is called on every game's cycle.
    // Purpose: update the internal components, logic, etc.
    void Update() {
        canvas->Update();
    }

    // This function is called on every game's cycle.
    // Purpose: render game's elements, components, text, etc.
    void Render() {
        rlImGuiBegin();

        ClearBackground(DARKBLUE);
        canvas->Render();

        rlImGuiEnd();
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
    // Creating the game instnace
    Game game;

    return 0;
}
