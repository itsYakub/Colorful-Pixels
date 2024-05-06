#include "IO.hpp"

#include <cstring>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <memory>

#include "ColorSystem.hpp"
#include "LayerSystem.hpp"
#include "Project.hpp"
#include "ToolSystem.hpp"
#include "imgui.h"
#include "raylib.h"
#include "raymath.h"
#include "nlohmann/json.hpp"

#include "Canvas.hpp"
#include "Layer.hpp"
#include "ColorfulPixels.hpp"
#include "IconsLucide.h"

IO::IO() :
    drawIntroGuiPanel(true),
    drawNewProjectGuiPanel(false),
    drawLoadProjectGuiPanel(false),
    drawSaveProjectGuiPanel(false),
    drawExportProjectGuiPanel(false) { }

void IO::NewProject(Project& project) {
    ImGui::SetNextWindowSize(ImVec2(256.0f, 216.0f));
    if(ImGui::Begin(ICON_LC_PLUS " Create new project...", &drawNewProjectGuiPanel, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize)) {
        static char title[128] = "hello_world";
        static int width = 32;
        static int height = 32;

        ImGui::InputText(":Title", title, 127);

        ImGui::InputInt(":Width", &width);
        ImGui::InputInt(":Height", &height);

        ImGui::SeparatorText("##separator");

        if(ImGui::Button(ICON_LC_FILE_PLUS_2 " Create") || ImGui::IsKeyReleased(ImGuiKey_Enter)) {
            project.Load(title, width, height);

            drawNewProjectGuiPanel = false;
            drawIntroGuiPanel = false;
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
    ImGui::SetNextWindowSize(ImVec2(512.0f, 128.0f));
    if(ImGui::Begin(ICON_LC_IMAGE_DOWN " Load project...", &drawLoadProjectGuiPanel, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize)) {
        static char path[256];

        ImGui::PushItemWidth(384.0f);
        ImGui::InputText(": Path", path, 255);
        ImGui::PopItemWidth();

        if(ImGui::Button(ICON_LC_IMAGE_DOWN " ")) {
            drawLoadProjectGuiPanel = false;
            drawIntroGuiPanel = false;
            DeserializeProject(project, path);

            ImGui::End();
            return;
        }

        ImGui::SameLine();

        if(ImGui::Button(ICON_LC_CIRCLE_X " Cancel") || ImGui::IsKeyReleased(ImGuiKey_Escape)) {
            drawLoadProjectGuiPanel = false;
            ImGui::End();
            return;

        }

        ImGui::End();
    }
}

void IO::SaveProject(Project& project) {
    ImGui::SetNextWindowSize(ImVec2(512.0f, 128.0f));
    if(ImGui::Begin(ICON_LC_IMAGE_DOWN " Save project...", &drawSaveProjectGuiPanel, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize)) {
        static char path[256];

        ImGui::PushItemWidth(384.0f);
        ImGui::InputText(": Path", path, 255);
        ImGui::PopItemWidth();

        if(ImGui::Button(ICON_LC_IMAGE_DOWN " ")) {
            drawSaveProjectGuiPanel = false;
            SerializeProject(project, path);

            ImGui::End();
            return;
        }

        ImGui::SameLine();

        if(ImGui::Button(ICON_LC_CIRCLE_X " Cancel") || ImGui::IsKeyReleased(ImGuiKey_Escape)) {
            drawSaveProjectGuiPanel = false;
            ImGui::End();
            return;

        }

        ImGui::End();
    }
}

void IO::ExportProject(Project& project, LayerSystem& layerSystem) {
    ImGui::SetNextWindowSize(ImVec2(512.0f, 128.0f));
    if(ImGui::Begin(ICON_LC_IMAGE_DOWN " Export image...", &drawExportProjectGuiPanel, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize)) {
        static char path[256];

        ImGui::PushItemWidth(384.0f);
        ImGui::InputText(": Path", path, 255);
        ImGui::PopItemWidth();

        if(ImGui::Button(ICON_LC_IMAGE_DOWN " Export to PNG")) {
            ExportImageLogic(project, layerSystem, path, "png");

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

void IO::IOGuiMenuItem(const char* title, bool draw, Project& project) {
    if(ImGui::BeginMenu(title, draw)) {
        if(ImGui::MenuItem(ICON_LC_FILE_PLUS " New...")) {
            TraceLog(LOG_INFO, "Menu option: New");
            drawNewProjectGuiPanel = true;
        }

        if(ImGui::MenuItem(ICON_LC_FILE_UP " Load...")) {
            TraceLog(LOG_INFO, "Menu option: Load");
            drawLoadProjectGuiPanel = true;
        }

        if(ImGui::MenuItem(ICON_LC_SAVE " Save...")) {
            if(project.valid) {
                TraceLog(LOG_INFO, "Menu option: Save");
                drawSaveProjectGuiPanel = true;
            } else {
                TraceLog(LOG_WARNING, "There isn't any valid project opened!");
            }
        }

        if(ImGui::MenuItem(ICON_LC_IMAGE_DOWN " Export...")) {
            if(project.valid) {
                TraceLog(LOG_INFO, "Menu option: Export");
                drawExportProjectGuiPanel = true;
            } else {
                TraceLog(LOG_WARNING, "There isn't any valid project opened!");
            }
        }

        ImGui::EndMenu();
    }
}

void IO::SerializeProject(Project& project, const std::string& path) {
    nlohmann::json projectJsonFile;

    project.IOSave(projectJsonFile);

    std::ofstream projectFile(TextFormat("%s", path.c_str()));
    projectFile << std::setw(4) << projectJsonFile << std::endl;
    projectFile.close();
}

void IO::DeserializeProject(Project& project, const std::string& path) {
    std::ifstream projectFile(path);
    nlohmann::json projectJsonFile = nlohmann::json::parse(projectFile);
    projectFile.close();

    project.IOLoad(projectJsonFile);
}

void IO::ExportImageLogic(Project& project, LayerSystem& layerSystem, const std::string& path, const char* format) {
    if(
        strcmp(format, "png") != 0
    ) {
        TraceLog(LOG_ERROR, TextFormat("%s isn't a valid image format.", format));
        
        return;
    }

    int w = layerSystem.GetLayer().GetSizeX();
    int h = layerSystem.GetLayer().GetSizeY();
    PixelData exportPixelData(w * h);

    for(int i = layerSystem.GetCount() - 1; i >= 0; i--) {
        auto& layer = layerSystem.GetLayer(i);

        for(int y = 0; y < h; y++) {
            for(int x = 0; x < w; x++) {
                if(ColorToInt(layer.GetPixelColor(x, y)) == ColorToInt(BLANK)) {
                    continue;
                }

                Layer::SetPixelColor(exportPixelData, x, y, w, h, layer.GetPixelColor(x, y));
            }
        }
    }

    Image resultImage = GenImageColor(
        layerSystem.GetLayer().GetSizeX(), 
        layerSystem.GetLayer().GetSizeX(), 
        BLANK
    );

    memcpy(
        resultImage.data, 
        exportPixelData.data(), 
        w * h * sizeof(Color)
    );

    ExportImage(
        resultImage, 
        TextFormat("%s%s.%s", path.c_str(), project.title.c_str(), format)
    );

    UnloadImage(resultImage);
}

