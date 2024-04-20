#include "Layer.hpp"

Layer::Layer(const int CELL_COUNT_X, const int CELL_COUNT_Y, int ID, bool visibility, bool lock) : 
    COUNT((const Vector2) { static_cast<float>(CELL_COUNT_X), static_cast<float>(CELL_COUNT_Y) } ), 
    ID(ID),
    m_LayerData(CELL_COUNT_X * CELL_COUNT_Y),
    layerVisible(visibility),
    layerLocked(lock) {
        Load();
}

Layer::Layer(const int CELL_COUNT, int ID) : Layer(CELL_COUNT, CELL_COUNT, ID, true, false) { }
Layer::Layer(const int CELL_COUNT, int ID, bool visibility) : Layer(CELL_COUNT, CELL_COUNT, ID, visibility, false) { }
Layer::Layer(const int CELL_COUNT, int ID, bool visibility, bool lock) : Layer(CELL_COUNT, CELL_COUNT, ID, visibility, lock) { }
Layer::Layer(const int CELL_COUNT_X, const int CELL_COUNT_Y, int ID) : Layer(CELL_COUNT_X, CELL_COUNT_Y, ID, true, false) { }
Layer::Layer(const int CELL_COUNT_X, const int CELL_COUNT_Y, int ID, bool visibility) : Layer(CELL_COUNT_X, CELL_COUNT_Y, ID, visibility, false) { }

void Layer::Load() {
    for(int i = 0; i < COUNT.x * COUNT.y; i++) {
        m_LayerData[i] = BLANK;
    }  

    Image image = GenImageColor(COUNT.x, COUNT.y, BLANK);
    m_LayerTexture = LoadTextureFromImage(image);
    SetTextureFilter(m_LayerTexture, TEXTURE_FILTER_POINT);
    UnloadImage(image);  
}

void Layer::Unload() {
    UnloadTexture(m_LayerTexture);
}

int Layer::GetID() {
    return ID;
}

Texture2D& Layer::GetTexture() {
    return m_LayerTexture;
}

std::vector<Color>& Layer::GetData() {
    return m_LayerData;
}

bool Layer::IsVisible() {
    return layerVisible;
}

void Layer::SetVisibility(bool visibility) {
    layerVisible = visibility;
}

bool Layer::IsLocked() {
    return layerLocked;
}

void Layer::SetLock(bool lock) {
    layerLocked = lock;
}

Color Layer::GetPixelColor(int x, int y) {
    return m_LayerData.at(y * COUNT.x + x);
}

void Layer::SetPixelColor(int x, int y, Color color) {
    x = Clamp(x, 0, COUNT.x - 1);
    y = Clamp(y, 0, COUNT.y - 1);

    m_LayerData.at(y * COUNT.x + x) = color;
}

void Layer::UpdateLayer() {
    UpdateTexture(m_LayerTexture, m_LayerData.data());
}
