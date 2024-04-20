#pragma once

#include <vector>
#include <memory>

#include "imgui.h"

#include "Layer.hpp"

class Canvas;

class LayerSystem {
private:
    Canvas* m_Canvas;

    std::vector<std::unique_ptr<Layer>> m_LayerList;
    int m_CurrentLayerID;
    int m_LayerCount;
    int m_LayerCountTotal;

public:
    LayerSystem(Canvas* canvas);

    void Unload();

    void UpdateLayer();

    int GetCurrentLayerIndex();
    void SetCurrentLayerIndex(int index);
    void IncrementCurrentLayerIndex();
    void DecrementCurrentLayerIndex();

    std::unique_ptr<Layer>& GetLayer();
    std::unique_ptr<Layer>& GetLayer(int index);

    int GetCount();

    void PushNewLayer(const int CELL_COUNT_X, const int CELL_COUNT_Y);
    void PopLayer();
    void EraseLayer();

    void LayersGuiPanel(const char* name, ImVec2 position, ImVec2 size);
};