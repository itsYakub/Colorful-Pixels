#pragma once

#include <string>

#include "LayerSystem.hpp"
#include "Project.hpp"

class IO {
public:
    bool drawIntroGuiPanel;
    bool drawNewProjectGuiPanel;
    bool drawSaveProjectGuiPanel;
    bool drawSaveAsProjectGuiPanel;
    bool drawLoadProjectGuiPanel;
    bool drawExportProjectGuiPanel;

public:
    IO();

    void NewProject(Project& project);
    void LoadProject(Project& project);
    void SaveProject(Project& project);
    void SaveAsProject(Project& project);
    void ExportProject(Project& project, LayerSystem& layerSystem);

    void IOGuiMenuItem(const char* title, bool draw, Project& project);

private:
    void SerializeProject(Project& project, const std::string& path);
    void DeserializeProject(Project& project, const std::string& path);
    void ExportImageLogic(Project& project, LayerSystem& layerSystem, const std::string& path, const char* format);
};