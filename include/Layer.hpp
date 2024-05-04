#pragma once

#include <vector>

#include "raylib.h"
#include "raymath.h"

using PixelData = std::vector<Color>;

class Layer {
private:
    const Vector2 COUNT;
    const int ID;

    PixelData m_LayerData;
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
    PixelData& GetData();
    Vector2 GetCount();

    bool IsVisible();
    void SetVisibility(bool visibility);

    bool IsLocked();
    void SetLock(bool lock);

    Color GetPixelColor(int x, int y);
    Color GetPixelColor(int i);
    void SetPixelColor(int x, int y, Color color);
    static void SetPixelColor(PixelData& pixelData, int x, int y, int w, int h, Color color);

    void UpdateLayer();
};