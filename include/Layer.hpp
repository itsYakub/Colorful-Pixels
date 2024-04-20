#pragma once

#include <vector>

#include "raylib.h"
#include "raymath.h"

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
    Layer(const int CELL_COUNT_X, const int CELL_COUNT_Y, int ID, bool visibility, bool lock);
    Layer(const int CELL_COUNT, int ID);
    Layer(const int CELL_COUNT, int ID, bool visibility);
    Layer(const int CELL_COUNT, int ID, bool visibility, bool lock);
    Layer(const int CELL_COUNT_X, const int CELL_COUNT_Y, int ID);
    Layer(const int CELL_COUNT_X, const int CELL_COUNT_Y, int ID, bool visibility);

    void Load();
    void Unload();

    int GetID();
    Texture2D& GetTexture();
    std::vector<Color>& GetData();

    bool IsVisible();
    void SetVisibility(bool visibility);

    bool IsLocked();
    void SetLock(bool lock);

    Color GetPixelColor(int x, int y);
    void SetPixelColor(int x, int y, Color color);

    void UpdateLayer();
};