#include "IO.hpp"

#include <cstring>
#include <filesystem>
#include <fstream>
#include <iomanip>

#include "IconsLucide.h"
#include "nlohmann/json.hpp"

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

void IO::SerializeProject(Project& project, const std::string& path) {
    nlohmann::json projectJsonFile;

    // When we serialize the project, we only need to have the access to all the necessary data from the `project` object
    // Those are:
    // - camera;
    // - canvas;
    // - tool;
    // - layer system and it's layers
    // - project's name and path
    // We can also throw a warning when the save file is from the older / newer version of CFPX because the file will store those informations!

    std::ofstream projectFile(path);
    projectFile << std::setw(4) << projectJsonFile << std::endl;
    projectFile.close();
}

void IO::DeserializeProject(Project& project, const std::string& path) {
    std::ifstream projectFile(path);
    nlohmann::json projectJsonFile = nlohmann::json::parse(projectFile);

    // To deserialize the save file we can create all the necessary components here
    // Those are:
    // - std::unique_ptr<Canvas>;
    // - std::unique_ptr<Tool>;
    // - LayerSystem;
    // - Camera2D;
    // Later on, during the final initialization, all of those components will be fed into the final Project class instance.

    projectFile.close();
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

