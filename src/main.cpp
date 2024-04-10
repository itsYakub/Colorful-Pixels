#include <string>
#include <cmath>
#include <memory>
#include <vector>

#include "raylib.h"
#include "raymath.h"

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
        m_LayerData.at(y * COUNT.y + x) = color;
    }

    void UpdateLayer() {
        UpdateTexture(m_LayerTexture, m_LayerData.data());
    }
};

class Canvas {
private:
    Vector2 m_Position;
    const Vector2 SIZE;
    const int CELL_COUNT_X;
    const int CELL_COUNT_Y;

    std::vector<std::unique_ptr<Layer>> m_LayerList;
    int m_CurrentLayerID;

public:
    Canvas() : 
        m_Position( (Vector2) { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f } ), 
        SIZE( (const Vector2) { 512.0f, 512.0f } ),
        CELL_COUNT_X(16), 
        CELL_COUNT_Y(16),
        m_LayerList(0),
        m_CurrentLayerID(0) {
            m_LayerList.push_back(std::make_unique<Layer>(CELL_COUNT_X, CELL_COUNT_Y, true, false));
    }

    Canvas(const int CELL_COUNT) : 
        m_Position( (Vector2) { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f } ), 
        SIZE( (const Vector2) { 512.0f, 512.0f } ),
        CELL_COUNT_X(CELL_COUNT), 
        CELL_COUNT_Y(CELL_COUNT),
        m_LayerList(0),
        m_CurrentLayerID(0) {
            m_LayerList.push_back(std::make_unique<Layer>(CELL_COUNT, true, false));
    }

    Canvas(const int CELL_COUNT_X, const int CELL_COUNT_Y) : 
        m_Position( (Vector2) { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f } ), 
        SIZE( (const Vector2) { 512.0f, 512.0f } ),
        CELL_COUNT_X(CELL_COUNT_X), 
        CELL_COUNT_Y(CELL_COUNT_Y),
        m_LayerList(0),
        m_CurrentLayerID(0) {
            m_LayerList.push_back(std::make_unique<Layer>(CELL_COUNT_X, CELL_COUNT_Y, true, false));
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

        m_LayerList.at(m_CurrentLayerID)->UpdateLayer();
    }

    void Render() {
        float originOffsetX = SIZE.x / 2.0f;
        float originOffsetY = SIZE.y / 2.0f;
    
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
                    SIZE.x, 
                    SIZE.y 
                },
                (Vector2) { 
                    originOffsetX, 
                    originOffsetY 
                },
                0.0f,
                WHITE
            );
        }

        for(int y = 0; y < CELL_COUNT_Y; y++) {
            for(int x = 0; x < CELL_COUNT_X; x++) {
                DrawRectangleLinesEx(
                    (Rectangle) { 
                        m_Position.x - originOffsetX + x * (SIZE.x / CELL_COUNT_X),
                        m_Position.y - originOffsetY + y * (SIZE.y / CELL_COUNT_Y),
                        SIZE.x / CELL_COUNT_X, 
                        SIZE.y / CELL_COUNT_Y 
                    }, 
                    1.0f, 
                    Fade(LIGHTGRAY, 0.5f)
                );
            }
        }
    }

private:
    Vector2 GetMouseCanvasIndex() {
        float originOffsetX = SIZE.x / 2.0f;
        float originOffsetY = SIZE.y / 2.0f;

        int canvasMouseRelationX = (GetMousePosition().x - m_Position.x + originOffsetX) / SIZE.x * CELL_COUNT_X;
        int canvasMouseRelationY = (GetMousePosition().y - m_Position.y + originOffsetY) / SIZE.y * CELL_COUNT_Y;

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
    Game() : WIDTH(1024), HEIGHT(768), TITLE(TextFormat("Raylib %s - Pixel Editor %s", RAYLIB_VERSION, "1.0")) {
        // Initializing the Audio Device and creating a Window.
        InitAudioDevice();
        InitWindow(WIDTH, HEIGHT, TITLE.c_str());

        canvas = std::make_unique<Canvas>(16);

#ifdef PLATFORM_WEB
        // Passing the `UpdateRenderFrame` function with the argument `this` for this `Game` class instance.
        // Value 0 caps the game's framerate to the default browser's framerate.
        // Value 1 tells emscritpen to simulate the infinite loop.
        emscripten_set_main_loop_arg(UpdateRenderFrame, this, 0, 1);
#else   
        // Caping the FPS to 60.
        // You can also use the `SetConfigFlag(FLAG_VSYNC_HINT)` before creating the window to cap the framerate to your monitor's refresh rate.
        SetTargetFPS(60);
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
