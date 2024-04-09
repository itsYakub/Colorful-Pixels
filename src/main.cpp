#include <string>
#include <cmath>
#include <memory>
#include <vector>

#include "raylib.h"

#ifdef PLATFORM_WEB
    #include "emscripten.h"
#endif

class Layer {
private:
    const Vector2 m_Size;
    std::vector<Color> m_LayerData;
    Texture2D m_LayerTexture;
    
public:
    Layer(const int WIDTH, const int HEIGHT) : m_Size((const Vector2) { static_cast<float>(WIDTH), static_cast<float>(HEIGHT) } ), m_LayerData(WIDTH * HEIGHT) {
        for(int i = 0; i < WIDTH * HEIGHT; i++) {
            m_LayerData[i] = BLANK;
        }  

        Image image = GenImageColor(WIDTH, HEIGHT, BLANK);
        m_LayerTexture = LoadTextureFromImage(image);
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

    void SetPixelColor(int x, int y, Color color) {
        m_LayerData.at(y * m_Size.y + x) = color;
    }

    void UpdateLayer() {
        UpdateTexture(m_LayerTexture, m_LayerData.data());
    }
};

class Canvas {
private:
    Vector2 m_Position;
    const Vector2 m_Size;
    float m_Scale;

    Layer m_Layer;

public:
    Canvas(const int WIDTH, const int HEIGHT, float scale) : m_Position( (Vector2) { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f } ), m_Size( (const Vector2) { static_cast<float>(WIDTH), static_cast<float>(HEIGHT) } ), m_Scale(scale), m_Layer(WIDTH, HEIGHT) {

    }

    void Unload() {
        m_Layer.Unload();
    }

    void Update() {
        int canvasMouseRelationX = (GetMousePosition().x - m_Position.x + m_Layer.GetTexture().width * m_Scale / 2.0f) / m_Scale;
        int canvasMouseRelationY = (GetMousePosition().y - m_Position.y + m_Layer.GetTexture().height * m_Scale / 2.0f) / m_Scale;

        bool canvasMouseRelationValid = 
            (canvasMouseRelationX >= 0 && canvasMouseRelationX < m_Size.x) &&
            (canvasMouseRelationY >= 0 && canvasMouseRelationY < m_Size.y); 


        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if(canvasMouseRelationValid) {
                m_Layer.SetPixelColor(canvasMouseRelationX, canvasMouseRelationY, WHITE);
            }
        }

        if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            if(canvasMouseRelationValid) {
                m_Layer.SetPixelColor(canvasMouseRelationX, canvasMouseRelationY, BLANK);
            }
        }

        m_Scale += GetMouseWheelMove();

        m_Layer.UpdateLayer();
    }

    void Render() {
        DrawTexturePro(
            m_Layer.GetTexture(),
            (Rectangle) { 0.0f, 0.0f, static_cast<float>(m_Layer.GetTexture().width), static_cast<float>(m_Layer.GetTexture().height) },
            (Rectangle) { m_Position.x, m_Position.y, static_cast<float>(m_Layer.GetTexture().width) * m_Scale, static_cast<float>(m_Layer.GetTexture().height) * m_Scale },
            (Vector2) { m_Layer.GetTexture().width * m_Scale / 2.0f, m_Layer.GetTexture().height * m_Scale / 2.0f },
            0.0f,
            WHITE
        );

        for(int y = 0; y < m_Size.y; y++) {
            for(int x = 0; x < m_Size.x; x++) {
                DrawRectangleLinesEx(
                    (Rectangle) { 
                        x * m_Scale + m_Position.x - m_Layer.GetTexture().width * m_Scale / 2.0f,
                        y * m_Scale + m_Position.y - m_Layer.GetTexture().height * m_Scale / 2.0f, 
                        m_Layer.GetTexture().width * m_Scale / m_Size.x, 
                        m_Layer.GetTexture().height * m_Scale / m_Size.y 
                    }, 
                    1.0f, 
                    Fade(LIGHTGRAY, 0.5f)
                );
            }
        }
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

        canvas = std::make_unique<Canvas>(32, 32, 16);

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
        ClearBackground(BLACK);
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
