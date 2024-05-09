#pragma once

#include <vector>
#include <memory>

#include "imgui.h"

#include "Layer.hpp"

using LayerList = std::vector<Layer>;

class LayerSystem {
private:
    LayerList m_LayerList;

    int m_CurrentLayerID;
    int m_LayerCount;
    int m_LayerCountTotal;

    const int CELL_COUNT_X;
    const int CELL_COUNT_Y;

public:
    LayerSystem(const int& CELL_COUNT_X, const int& CELL_COUNT_Y, bool createFirstLayer);

    void Unload();

    void UpdateLayer();
    void UpdateLayer(int index);

    LayerList& GetList();
    Layer& GetLayer();
    Layer& GetLayer(int index);

    int GetCount();

    void PushNewLayer(const int CELL_COUNT_X, const int CELL_COUNT_Y);
    void PushNewLayer(const int CELL_COUNT_X, const int CELL_COUNT_Y, int ID, bool visibility, bool lock);
    void InsertNewLayer(const int CELL_COUNT_X, const int CELL_COUNT_Y);
    void InsertNewLayer(int index, const int CELL_COUNT_X, const int CELL_COUNT_Y);
    void InsertNewLayer(const int CELL_COUNT_X, const int CELL_COUNT_Y, int ID, bool visibility, bool lock);
    void InsertNewLayer(int index, const int CELL_COUNT_X, const int CELL_COUNT_Y, int ID, bool visibility, bool lock);
    void PopLayer();
    void EraseLayer();

    void LayersGuiPanel(const char* name, bool draw);
};