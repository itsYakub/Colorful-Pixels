#pragma once

#include <string>

#include "LayerSystem.hpp"
#include "Project.hpp"

class IO {
public:
    bool drawNewProjectGuiPanel;
    bool drawExportProjectGuiPanel;

public:
    IO();

    void NewProject(Project& project);
    void LoadProject(Project& project);
    void SaveProject(Project& project);
    void ExportProject(Project& project, LayerSystem& layerSystem);

private:
    void SerializeProject(Project& project);
    void DeserializeProject(const std::string& path);
    void ExportImageLogic(Project& project, LayerSystem& layerSystem, const char* format);
};