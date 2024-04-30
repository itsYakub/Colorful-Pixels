#pragma once

#include "LayerSystem.hpp"
#include "Project.hpp"

class IO {
private:

public:
    IO();

    void NewProject(Project& project);
    void LoadProject(Project& project);
    void SaveProject(Project& project);
    void ExportProject(LayerSystem& layerSystem);

private:

};