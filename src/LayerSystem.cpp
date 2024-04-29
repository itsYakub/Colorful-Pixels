#include "LayerSystem.hpp"

#include "IconsLucide.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "rlImGui.h"

#include "IconsLucide.h"

LayerSystem::LayerSystem(const int& CELL_COUNT_X, const int& CELL_COUNT_Y) :
    m_LayerList(0),
    m_CurrentLayerID(0),
    m_LayerCount(0),
    m_LayerCountTotal(0),
    CELL_COUNT_X(CELL_COUNT_X),
    CELL_COUNT_Y(CELL_COUNT_Y) {
        PushNewLayer(CELL_COUNT_X, CELL_COUNT_Y);
}

void LayerSystem::Unload() {
    for(auto& i : m_LayerList) {
        i->Unload();
    }
}

void LayerSystem::UpdateLayer() {
    m_LayerList.at(m_CurrentLayerID)->UpdateLayer();
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

    if(m_CurrentLayerID < m_LayerList.size() - 1) {
        m_CurrentLayerID++;
    }

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

    if(ImGui::Begin(name, nullptr)) {
        if(ImGui::Button("Add layer")) {
            PushNewLayer(CELL_COUNT_X, CELL_COUNT_Y);
        }

        ImGui::SameLine();

        if(ImGui::Button("Remove layer") && m_LayerList.size() > 1) {
            EraseLayer();
        }

        ImGui::SameLine();

        if(ImGui::Button("Pop layer") && m_LayerList.size() > 1) {
            PopLayer();
        }

        ImGui::SeparatorText("##text");

        for(int i = m_LayerCount - 1; i >= 0; i--) {
            ImGui::PushID(i);
                if(ImGui::Button(m_CurrentLayerID == i ? TextFormat("Layer %i. (Current)", m_LayerList.at(i)->GetID()) : TextFormat("Layer %i.", m_LayerList.at(i)->GetID()), ImVec2(256.0f, 38.0f))) {
                    m_CurrentLayerID = i;
                }

            ImGui::SameLine();

                if(rlImGuiImageButtonSize("##layer_preview", &GetLayer(i)->GetTexture(), ImVec2(32, 32))) {
                    m_CurrentLayerID = i;
                }
                
            ImGui::PopID();
            
            ImGui::SameLine();
            ImGui::PushID(i);

                if(ImGui::Button(GetLayer()->layerVisible ? ICON_LC_EYE : ICON_LC_EYE_OFF, ImVec2(38, 38))) {
                    m_LayerList.at(i)->layerVisible = !m_LayerList.at(i)->layerVisible;
                }

            ImGui::PopID();

            ImGui::SameLine();
            ImGui::PushID(i);

                if(ImGui::Button(GetLayer()->layerLocked ? ICON_LC_LOCK : ICON_LC_LOCK_OPEN, ImVec2(38, 38))) {
                    m_LayerList.at(i)->layerLocked = !m_LayerList.at(i)->layerLocked;
                }

            ImGui::PopID();

            ImGui::SameLine();
            ImGui::PushID(i);

                if(ImGui::Button(ICON_LC_SQUARE_CHEVRON_UP, ImVec2(38, 38))) {
                    if(i - 1 >= 0) {
                        std::swap(m_LayerList.at(i), m_LayerList.at(i - 1));

                        if(m_CurrentLayerID == i) {
                            m_CurrentLayerID--;
                        }
                    }
                }

            ImGui::PopID();

            ImGui::SameLine();
            ImGui::PushID(i);
            
                if(ImGui::Button(ICON_LC_SQUARE_CHEVRON_DOWN, ImVec2(38, 38))) {
                    if(i + 1 < m_LayerList.size()) {
                        std::swap(m_LayerList.at(i), m_LayerList.at(i + 1));

                        if(m_CurrentLayerID == i) {
                            m_CurrentLayerID++;
                        }
                    }
                }

            ImGui::PopID();
        }

        ImGui::End();
    }
}
