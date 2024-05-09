#pragma once

#include "raylib.h"

#include "Layer.hpp"

class Algorithms {
public:
    static void DigitalDifferentialAnalyzer(int ax, int ay, int bx, int by, Color color, Layer& layer);
    static void BresenhamLineAnalyzer(int ax, int ay, int bx, int by, Color color, Layer& layer);

    static void FloodFillAlgorithm(int x, int y, Color oldColor, Color newColor, Layer& layer);

private:
    static void Fill(int x, int y, Color oldColor, Color newColor, Layer& layer);
};