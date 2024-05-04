#include "Layer.hpp"

Layer::Layer(const int CELL_COUNT_X, const int CELL_COUNT_Y, int ID, bool visibility, bool lock) : 
    m_XSize(CELL_COUNT_X),
    m_YSize(CELL_COUNT_Y), 
    m_Id(ID),
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
    for(int i = 0; i < m_XSize * m_YSize; i++) {
        m_LayerData[i] = BLANK;
    }  

    Image image = GenImageColor(m_XSize, m_YSize, BLANK);
    m_LayerTexture = LoadTextureFromImage(image);
    SetTextureFilter(m_LayerTexture, TEXTURE_FILTER_POINT);
    UnloadImage(image);  
}

void Layer::Unload() {
    UnloadTexture(m_LayerTexture);
}

int Layer::GetSizeX() {
    return m_XSize;
}
int Layer::GetSizeY() {
    return m_YSize;
}
int Layer::GetID() {
    return m_Id;
}


Texture2D& Layer::GetTexture() {
    return m_LayerTexture;
}

PixelData& Layer::GetData() {
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
    return m_LayerData.at(y * m_XSize + x);
}

Color Layer::GetPixelColor(int i) {
    return m_LayerData.at(i);
}

void Layer::SetPixelColor(int x, int y, Color color) {
    x = Clamp(x, 0, m_XSize - 1);
    y = Clamp(y, 0, m_YSize - 1);

    m_LayerData[y * m_XSize + x] = color;
}

void Layer::SetPixelColor(PixelData& pixelData, int x, int y, int w, int h, Color color) {
    x = Clamp(x, 0, w - 1);
    y = Clamp(y, 0, h - 1);

    pixelData[y * w + x] = color;
}

void Layer::UpdateLayer() {
    UpdateTexture(m_LayerTexture, m_LayerData.data());
}
