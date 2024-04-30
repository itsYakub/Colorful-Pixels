#include "IO.hpp"

#include <cstring>

#include "IconsLucide.h"

#include "Layer.hpp"
#include "imgui.h"
#include "raylib.h"
#include "raymath.h"

IO::IO() :
    drawNewProjectGuiPanel(false),
    drawExportProjectGuiPanel(false) { }

void IO::NewProject(Project& project) {
    if(ImGui::Begin(ICON_LC_PLUS " Create new project...", &drawNewProjectGuiPanel, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize)) {
        static char title[128] = "Project";
        static char path[256];
        static int width = 32;
        static int height = 32;

        strcpy(path, GetApplicationDirectory());

        ImGui::InputText(":Title", title, 127);

#ifndef PLATFORM_WEB
        ImGui::InputText(":Path", path, 255);
#endif

        ImGui::InputInt(":Width", &width);
        project.width = Clamp(width, 2, 128);

        ImGui::InputInt(":Height", &height);
        project.height = Clamp(height, 2, 128);

        if(ImGui::Button(ICON_LC_FILE_PLUS_2 " Create") || ImGui::IsKeyReleased(ImGuiKey_Enter)) {
            project.Load(title, path, width, width);

            drawNewProjectGuiPanel = false;
            ImGui::End();
            return;
        }

        ImGui::SameLine();

        if(ImGui::Button(ICON_LC_CIRCLE_X " Cancel") || ImGui::IsKeyReleased(ImGuiKey_Escape)) {
            drawNewProjectGuiPanel = false;
            ImGui::End();
            return;
        }

        ImGui::End();
    }
}

void IO::LoadProject(Project& project) {
    // TODO: Fill me
}

void IO::SaveProject(Project& project) {
    // TODO: Fill me daddy
}

void IO::ExportProject(Project& project, LayerSystem& layerSystem) {
    if(ImGui::Begin(ICON_LC_IMAGE_DOWN " Export image...", &drawExportProjectGuiPanel, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize)) {
        if(ImGui::Button(ICON_LC_IMAGE_DOWN " Export to PNG")) {
            ExportImageLogic(project, layerSystem, "png");

            drawExportProjectGuiPanel = false;
            ImGui::End();
            return;
        }

        ImGui::SameLine();

        if(ImGui::Button(ICON_LC_CIRCLE_X " Cancel") || ImGui::IsKeyReleased(ImGuiKey_Escape)) {
            drawExportProjectGuiPanel = false;
            ImGui::End();
            return;

        }

        ImGui::End();
    }
}

void IO::SerializeProject(Project& project) {

}

void IO::DeserializeProject(const std::string& path) {

}

void IO::ExportImageLogic(Project& project, LayerSystem& layerSystem, const char* format) {
    if(
        strcmp(format, "png") != 0
    ) {
        TraceLog(LOG_ERROR, TextFormat("%s isn't a valid image format.", format));
        
        return;
    }

    int w = layerSystem.GetLayer()->GetCount().x;
    int h = layerSystem.GetLayer()->GetCount().y;
    PixelData exportPixelData(w * h);

    for(int i = layerSystem.GetCount() - 1; i >= 0; i--) {
        auto& layer = layerSystem.GetLayer(i);

        for(int y = 0; y < h; y++) {
            for(int x = 0; x < w; x++) {
                if(ColorToInt(layer->GetPixelColor(x, y)) == ColorToInt(BLANK)) {
                    continue;
                }

                Layer::SetPixelColor(exportPixelData, x, y, w, h, layer->GetPixelColor(x, y));
            }
        }
    }

    Image resultImage = GenImageColor(
        layerSystem.GetLayer()->GetCount().x, 
        layerSystem.GetLayer()->GetCount().x, 
        BLANK
    );

    memcpy(
        resultImage.data, 
        exportPixelData.data(), 
        w * h * sizeof(Color)
    );

    ExportImage(
        resultImage, 
        TextFormat("%s%s.%s", project.path.c_str(), project.title.c_str(), format)
    );

    UnloadImage(resultImage);
}

