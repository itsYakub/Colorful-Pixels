#include "ColorSystem.hpp"

ColorSystem::ColorSystem() :
    m_ColorPalette(),
    m_CurrentColor(WHITE) {

}

ColorPalette& ColorSystem::GetPalette() {
    return m_ColorPalette;
}

void ColorSystem::SetPalette(ColorPalette& newPalette) {
    m_ColorPalette = newPalette;
}

Color& ColorSystem::GetColor() {
    return m_CurrentColor;
}

void ColorSystem::SetColor(Color newColor) {
    m_CurrentColor = newColor;
}

void ColorSystem::SetColor(float newColor[4]) {
    m_CurrentColor = {
        static_cast<unsigned char>(newColor[0] * 255),
        static_cast<unsigned char>(newColor[1] * 255),
        static_cast<unsigned char>(newColor[2] * 255),
        static_cast<unsigned char>(newColor[3] * 255),
    };
}

void ColorSystem::ColorGuiPanel(const char* name, ImVec2 position, ImVec2 size) {
    ImGui::Begin(
        name, 
        NULL, 
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration
    );

    ImGui::SetWindowPos(position);
    ImGui::SetWindowSize(size);

    float colorArray[4] = { m_CurrentColor.r / 255.0f, m_CurrentColor.g / 255.0f, m_CurrentColor.b / 255.0f, m_CurrentColor.a / 255.0f };

    ImGui::SeparatorText("Color Picker");

    ImGui::PushItemWidth(size.x - 16.0f);
    ImGui::ColorPicker4(
        "Color Picker", 
        colorArray, 
        ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoLabel
    );

    SetColor(colorArray);

    ImGui::SeparatorText("Color Palette");

    ImGui::BeginChild(1);

    for(int i = 0, colorsInARow = 4; i < m_ColorPalette.Size(); i += colorsInARow) {
        ImGui::NewLine();

        for(int j = 0; j < colorsInARow; j++) {
            if(i + j >= m_ColorPalette.Size()) {
                break;
            }

            Color colorPaletteColor = m_ColorPalette.GetColor(i + j);

            ImGui::SameLine();
            ImGui::PushID(i + j);
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(colorPaletteColor.r / 255.0f, colorPaletteColor.g / 255.0f, colorPaletteColor.b / 255.0f, 1.0f));
            if(ImGui::Button("##color_button", ImVec2(32, 32))) {
                m_CurrentColor = colorPaletteColor;
            }
            ImGui::PopStyleColor();
            ImGui::PopID();
        }
    }

    ImGui::EndChild();

    ImGui::End();

}
