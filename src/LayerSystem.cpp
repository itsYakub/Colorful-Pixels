#include "LayerSystem.hpp"

#include "Canvas.hpp"

LayerSystem::LayerSystem(Canvas* canvas) :
    m_Canvas(canvas),
    m_LayerList(0),
    m_CurrentLayerID(0),
    m_LayerCount(0),
    m_LayerCountTotal(0) {
        PushNewLayer(m_Canvas->CellCountX(), m_Canvas->CellCountY());
}

void LayerSystem::Unload() {
    for(auto& i : m_LayerList) {
        i->Unload();
    }
}

void LayerSystem::UpdateLayer() {
    m_LayerList.at(m_CurrentLayerID)->UpdateLayer();
}

int LayerSystem::GetCurrentLayerIndex() {
    return m_CurrentLayerID;
}

void LayerSystem::SetCurrentLayerIndex(int index) {
    if(index >= 0 && index < m_LayerList.size()) {
        m_CurrentLayerID = index;
    }
}

void LayerSystem::IncrementCurrentLayerIndex() {
    if(m_CurrentLayerID < m_LayerList.size() - 1) {
        m_CurrentLayerID++;
    }
}

void LayerSystem::DecrementCurrentLayerIndex() {
    if(m_CurrentLayerID >= 1) {
        m_CurrentLayerID--;
    }
}

std::unique_ptr<Layer>& LayerSystem::GetLayer() {
    return m_LayerList.at(m_CurrentLayerID);
}

std::unique_ptr<Layer>& LayerSystem::GetLayer(int index) {
    return m_LayerList.at(index);
}

int LayerSystem::GetCount(){
    return m_LayerCount;
}

void LayerSystem::PushNewLayer(const int CELL_COUNT_X, const int CELL_COUNT_Y) {
    m_LayerList.push_back(std::make_unique<Layer>(CELL_COUNT_X, CELL_COUNT_Y, m_LayerCountTotal, true, false));
    m_LayerCount++;
    m_LayerCountTotal++;

    IncrementCurrentLayerIndex();

    TraceLog(LOG_INFO, TextFormat("Inserted the layer no. %i", m_LayerCountTotal));
}

void LayerSystem::PopLayer() {
    m_LayerList.pop_back();
    m_LayerCount--;
    
    if(m_CurrentLayerID >= m_LayerList.size()) {
        m_CurrentLayerID--;
    }
}

void LayerSystem::EraseLayer() {
    m_LayerList.erase(m_LayerList.begin() + m_CurrentLayerID);

    if(m_CurrentLayerID < 0) {
        m_CurrentLayerID++;
    } else if(m_CurrentLayerID >= m_LayerList.size()) {
        m_CurrentLayerID--;
    }

    m_LayerCount--;
}

void LayerSystem::LayersGuiPanel(const char* name, bool draw) {
    if(!draw) {
        return;
    }

    ImGui::Begin(
        name, 
        nullptr
    );

    if(ImGui::Button("Add layer")) {
        PushNewLayer(m_Canvas->CellCountX(), m_Canvas->CellCountY());
    }

    ImGui::SameLine();

    if(ImGui::Button("Remove layer") && m_LayerList.size() > 1) {
        EraseLayer();
    }

    ImGui::SameLine();

    if(ImGui::Button("Pop layer") && m_LayerList.size() > 1) {
        PopLayer();
    }

    ImGui::Separator();

    ImGui::BeginChild(1);

    for(int i = m_LayerCount - 1; i >= 0; i--) {
        if(ImGui::Button(m_CurrentLayerID == i ? TextFormat("Layer no.%i (Current)", m_LayerList.at(i)->GetID()) : TextFormat("Layer no.%i", m_LayerList.at(i)->GetID()), ImVec2(256.0f, 20.0f))) {
            m_CurrentLayerID = i;
        }

        ImGui::SameLine();
        ImGui::PushID(i);
            ImGui::Checkbox("##visible", &m_LayerList.at(i)->layerVisible);
        ImGui::PopID();

        ImGui::SameLine();
        ImGui::PushID(i);
            ImGui::Checkbox("##locked", &m_LayerList.at(i)->layerLocked);
        ImGui::PopID();

        ImGui::SameLine();
        ImGui::PushID(i);

            if(ImGui::Button("Up")) {
                if(i - 1 >= 0) {
                    std::swap(m_LayerList.at(i), m_LayerList.at(i - 1));
                }
            }

        ImGui::PopID();

        ImGui::SameLine();
        ImGui::PushID(i);
        
            if(ImGui::Button("Down")) {
                if(i + 1 < m_LayerList.size()) {
                    std::swap(m_LayerList.at(i), m_LayerList.at(i + 1));
                }
            }

        ImGui::PopID();
    }

    ImGui::EndChild();
    ImGui::End();    

}
