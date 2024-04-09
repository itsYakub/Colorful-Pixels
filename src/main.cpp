#include <string>

#include "raylib.h"

#ifdef PLATFORM_WEB
    #include "emscripten.h"
#endif
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

public:
    // `Game` class constructor.
    Game() : WIDTH(1024), HEIGHT(768), TITLE(TextFormat("Raylib %s - Game %s", RAYLIB_VERSION, "1.0")) {
        // Initializing the Audio Device and creating a Window.
        InitAudioDevice();
        InitWindow(WIDTH, HEIGHT, TITLE.c_str());

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
        // Deinitializing the game's resources.
        CloseAudioDevice();
        CloseWindow();
    }

    // This function is called on every game's cycle.
    // Purpose: update the internal components, logic, etc.
    void Update() {

    }

    // This function is called on every game's cycle.
    // Purpose: render game's elements, components, text, etc.
    void Render() {
        ClearBackground(RAYWHITE);
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
