#include <string>
#include <cmath>
#include <memory>
#include <vector>

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#ifdef PLATFORM_WEB
    #include "emscripten.h"
#endif

class Layer {
private:
    const Vector2 COUNT;

    std::vector<Color> m_LayerData;
    Texture2D m_LayerTexture;

    bool m_LayerVisible;
    bool m_LayerLocked;
    
public:
    Layer(const int CELL_COUNT) : 
        COUNT((const Vector2) { static_cast<float>(CELL_COUNT), static_cast<float>(CELL_COUNT) } ), 
        m_LayerData(pow(CELL_COUNT, 2)),
        m_LayerVisible(true),
        m_LayerLocked(false) {
            Load();
    }

    Layer(const int CELL_COUNT, bool visibility) : 
        COUNT((const Vector2) { static_cast<float>(CELL_COUNT), static_cast<float>(CELL_COUNT) } ), 
        m_LayerData(pow(CELL_COUNT, 2)),
        m_LayerVisible(visibility),
        m_LayerLocked(false) {
            Load();
    }

    Layer(const int CELL_COUNT, bool visibility, bool lock) : 
        COUNT((const Vector2) { static_cast<float>(CELL_COUNT), static_cast<float>(CELL_COUNT) } ), 
        m_LayerData(pow(CELL_COUNT, 2)),
        m_LayerVisible(visibility),
        m_LayerLocked(lock) {
            Load();
    }

    Layer(const int CELL_COUNT_X, const int CELL_COUNT_Y) : 
        COUNT((const Vector2) { static_cast<float>(CELL_COUNT_X), static_cast<float>(CELL_COUNT_Y) } ), 
        m_LayerData(CELL_COUNT_X * CELL_COUNT_Y),
        m_LayerVisible(true),
        m_LayerLocked(false) {
            Load();
    }

    Layer(const int CELL_COUNT_X, const int CELL_COUNT_Y, bool visibility) : 
        COUNT((const Vector2) { static_cast<float>(CELL_COUNT_X), static_cast<float>(CELL_COUNT_Y) } ), 
        m_LayerData(CELL_COUNT_X * CELL_COUNT_Y),
        m_LayerVisible(visibility),
        m_LayerLocked(false) {
            Load();
    }

    Layer(const int CELL_COUNT_X, const int CELL_COUNT_Y, bool visibility, bool lock) : 
        COUNT((const Vector2) { static_cast<float>(CELL_COUNT_X), static_cast<float>(CELL_COUNT_Y) } ), 
        m_LayerData(CELL_COUNT_X * CELL_COUNT_Y),
        m_LayerVisible(visibility),
        m_LayerLocked(lock) {
            Load();
    }

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
        return m_LayerVisible;
    }

    void SetVisibility(bool visibility) {
        m_LayerVisible = visibility;
    }

    bool IsLocked() {
        return m_LayerLocked;
    }

    void SetLock(bool lock) {
        m_LayerLocked = lock;
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

public:
    Canvas() : 
        m_Camera( (Camera2D) { 0 }),
        m_CameraOffset( (const Vector2) { static_cast<float>(GetScreenWidth()) / 2.0f, static_cast<float>(GetScreenHeight()) / 2.0f } ),

        m_Position( (Vector2) { 0.0f, 0.0f } ), 
        SIZE(GetCanvasSize(16, 16)),

        CELL_COUNT_X(16), 
        CELL_COUNT_Y(16),

        m_Scale(1.0f),

        m_LayerList(0),
        m_CurrentLayerID(0) {
            m_LayerList.push_back(std::make_unique<Layer>(CELL_COUNT_X, CELL_COUNT_Y, true, false));

            m_Camera.target = m_Position;
            m_Camera.offset = m_CameraOffset;
            m_Camera.zoom = m_Scale;
    }

    Canvas(const int CELL_COUNT) : 
        m_Camera( (Camera2D) { 0 }),
        m_CameraOffset( (const Vector2) { static_cast<float>(GetScreenWidth()) / 2.0f, static_cast<float>(GetScreenHeight()) / 2.0f } ),

        m_Position( (Vector2) { 0.0f, 0.0f } ), 
        SIZE(GetCanvasSize(CELL_COUNT, CELL_COUNT)),

        CELL_COUNT_X(CELL_COUNT), 
        CELL_COUNT_Y(CELL_COUNT),

        m_Scale(1.0f),

        m_LayerList(0),
        m_CurrentLayerID(0) {
            m_LayerList.push_back(std::make_unique<Layer>(CELL_COUNT, true, false));

            m_Camera.target = m_Position;
            m_Camera.offset = m_CameraOffset;
            m_Camera.zoom = m_Scale;
    }

    Canvas(const int CELL_COUNT_X, const int CELL_COUNT_Y) : 
        m_Camera( (Camera2D) { 0 }),
        m_CameraOffset( (const Vector2) { static_cast<float>(GetScreenWidth()) / 2.0f, static_cast<float>(GetScreenHeight()) / 2.0f } ),

        m_Position( (Vector2) { 0.0f, 0.0f } ), 
        SIZE(GetCanvasSize(CELL_COUNT_X, CELL_COUNT_Y)),

        CELL_COUNT_X(CELL_COUNT_X), 
        CELL_COUNT_Y(CELL_COUNT_Y),

        m_Scale(1.0f),

        m_LayerList(0),
        m_CurrentLayerID(0) {
            m_LayerList.push_back(std::make_unique<Layer>(CELL_COUNT_X, CELL_COUNT_Y, true, false));

            m_Camera.target = m_Position;
            m_Camera.offset = m_CameraOffset;
            m_Camera.zoom = m_Scale;
    }

    void Unload() {
        for(auto& i : m_LayerList) {
            i->Unload();
        }
    }

    void Update() {
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if(IsMouseCanvasIndexValid() && !m_LayerList.at(m_CurrentLayerID)->IsLocked()) {
                m_LayerList.at(m_CurrentLayerID)->SetPixelColor(GetMouseCanvasIndex().x, GetMouseCanvasIndex().y, WHITE);
            }
        }

        if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            if(IsMouseCanvasIndexValid() && !m_LayerList.at(m_CurrentLayerID)->IsLocked()) {
                m_LayerList.at(m_CurrentLayerID)->SetPixelColor(GetMouseCanvasIndex().x, GetMouseCanvasIndex().y, BLANK);
            }
        }

        if(IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
            Vector2 delta = GetMouseDelta();
			delta = Vector2Scale(delta, -1.0f / m_Camera.zoom);

			m_Camera.target = Vector2Add(m_Camera.target, delta);
        }

        if(GetMouseWheelMove() != 0.0f) {
            m_Scale += GetMouseWheelMove() / 10.0f;
            m_Scale = Clamp(m_Scale, 0.5f, 2.0f);
			
            m_Camera.target = GetScreenToWorld2D(GetMousePosition(), m_Camera);
			m_Camera.offset = GetMousePosition();
            m_Camera.zoom = m_Scale;
        }

        m_LayerList.at(m_CurrentLayerID)->UpdateLayer();
    }

    void Render() {    
        BeginMode2D(m_Camera);

        for(const auto& layer : m_LayerList) {
            if(!layer->IsVisible()) {
                continue;
            }

            DrawTexturePro(
                layer->GetTexture(),
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

        for(int y = 0; y < CELL_COUNT_Y; y++) {
            for(int x = 0; x < CELL_COUNT_X; x++) {
                DrawRectangleLinesEx(
                    (Rectangle) { 
                        m_Position.x - GetCanvasOriginOffset().x + x * (SIZE.x * m_Scale / CELL_COUNT_X),
                        m_Position.y - GetCanvasOriginOffset().y + y * (SIZE.y * m_Scale / CELL_COUNT_Y),
                        SIZE.x * m_Scale / CELL_COUNT_X, 
                        SIZE.y * m_Scale / CELL_COUNT_Y 
                    }, 
                    1.0f, 
                    Fade(LIGHTGRAY, 0.5f)
                );
            }
        }

        DrawRectangleLinesEx(
            (Rectangle) {
                m_Position.x - GetCanvasOriginOffset().x,
                m_Position.y - GetCanvasOriginOffset().y,
                SIZE.x * m_Scale,
                SIZE.y * m_Scale
            },
            2.0f,
            BLACK
        );

        EndMode2D();
    }

    std::unique_ptr<Layer>& GetLayer() {
        return m_LayerList.at(m_CurrentLayerID);
    }

    std::unique_ptr<Layer>& GetLayer(int index) {
        return m_LayerList.at(index);
    }

private:
    Vector2 GetCanvasSize(const int COUNT_X, const int COUNT_Y) {
        const float defaultResolutionValue = 512.0f;

        if(COUNT_X > COUNT_Y) {
            Vector2 result = { 
                defaultResolutionValue, 
                (defaultResolutionValue / COUNT_X) * COUNT_Y 
            };

            TraceLog(LOG_INFO, TextFormat("Canvas size: %f / %f", result.x, result.y));

            return result;

        } else if(COUNT_X < COUNT_Y) {
            Vector2 result = { 
                (defaultResolutionValue / COUNT_Y) * COUNT_X, 
                defaultResolutionValue 
            };

            TraceLog(LOG_INFO, TextFormat("Canvas size: %f / %f", result.x, result.y));

            return result;

        } else {
            Vector2 result = { 
                defaultResolutionValue, 
                defaultResolutionValue 
            };
            
            TraceLog(LOG_INFO, TextFormat("Canvas size: %f / %f", result.x, result.y));

            return result;
        }
    }

    Vector2 GetCanvasOriginOffset() {
        float originOffsetX = SIZE.x * m_Scale / 2.0f;
        float originOffsetY = SIZE.y * m_Scale / 2.0f;

        return {
            originOffsetX,
            originOffsetY
        };
    }

    Vector2 GetMouseCanvasIndex() {
        Vector2 mousePositionInTheWorld {
            GetScreenToWorld2D(GetMousePosition(), m_Camera).x + GetCanvasOriginOffset().x,
            GetScreenToWorld2D(GetMousePosition(), m_Camera).y + GetCanvasOriginOffset().y
        };

        int canvasMouseRelationX = (mousePositionInTheWorld.x / m_Scale) / SIZE.x * CELL_COUNT_X;
        int canvasMouseRelationY = (mousePositionInTheWorld.y / m_Scale) / SIZE.y * CELL_COUNT_Y;

        return { 
            static_cast<float>(canvasMouseRelationX), 
            static_cast<float>(canvasMouseRelationY) 
        };
    }

    bool IsMouseCanvasIndexValid() {
        Vector2 mouseCavasInput = GetMouseCanvasIndex();

        return (mouseCavasInput.x >= 0 && mouseCavasInput.x < CELL_COUNT_X) &&
            (mouseCavasInput.y >= 0 && mouseCavasInput.y < CELL_COUNT_Y); 
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

        canvas = std::make_unique<Canvas>(9);

#ifdef PLATFORM_WEB
        // Passing the `UpdateRenderFrame` function with the argument `this` for this `Game` class instance.
        // Value 0 caps the game's framerate to the default browser's framerate.
        // Value 1 tells emscritpen to simulate the infinite loop.
        emscripten_set_main_loop_arg(UpdateRenderFrame, this, 0, 1);
#else   
        // Caping the FPS to 60.
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
        ClearBackground(DARKBLUE);
        canvas->Render();
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
