#pragma once

#include <string>

#include "LayerSystem.hpp"
#include "Project.hpp"

#define CFPX_CAMERA "camera"
#define CFPX_CAMERA_TARGET CFPX_CAMERA "_target"
#define CFPX_CAMERA_OFFSET CFPX_CAMERA "_offset"
#define CFPX_CAMERA_ZOOM CFPX_CAMERA "_zoom"

#define CFPX_CANVAS "canvas"
#define CFPX_CANVAS_WIDTH CFPX_CANVAS "_width"
#define CFPX_CANVAS_HEIGHT CFPX_CANVAS "_height"
#define CFPX_CANVAS_SCALE CFPX_CANVAS "_scale"

#define CFPX_LAYER "layer"
#define CFPX_LAYER_WIDTH CFPX_LAYER "_width"
#define CFPX_LAYER_HEIGHT CFPX_LAYER "_height"
#define CFPX_LAYER_COUNT CFPX_LAYER "_count"

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
    void SerializeProject(Project& project, const std::string& path);
    void DeserializeProject(Project& project, const std::string& path);
    void ExportImageLogic(Project& project, LayerSystem& layerSystem, const char* format);
};