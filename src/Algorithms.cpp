#include "Algorithms.hpp"
#include "raylib.h"

#include <cmath>

void Algorithms::DigitalDifferentialAnalyzer(int ax, int ay, int bx, int by, Color color, Layer& layer) {
    // Source: https://en.wikipedia.org/wiki/Digital_differential_analyzer_(graphics_algorithm)

    float dx = bx - ax;
    float dy = by - ay;
    float steps = 0;
    float i = 0;

    float x = 0;
    float y = 0;

    if (std::abs(dx) >= std::abs(dy)) {
        steps = std::abs(dx);
    } else {
        steps = std::abs(dy);
    }

    dx = dx / steps;
    dy = dy / steps;
    x = ax;
    y = ay;

    while (i <= steps) {
        layer.SetPixelColor(x, y, color);
        x += dx;
        y += dy;
        i++;
    }
}

void Algorithms::BresenhamLineAnalyzer(int ax, int ay, int bx, int by, Color color, Layer& layer) {

}

void Algorithms::FloodFillAlgorithm(int x, int y, Color oldColor, Color newColor, Layer& layer) {
    // Source: https://en.wikipedia.org/wiki/Flood_fill
    // Source: https://pl.wikipedia.org/wiki/Flood_fill
    // Source: https://www.youtube.com/watch?v=aehEcTEPtCs

    Fill(x, y, oldColor, newColor, layer);
}

void Algorithms::Fill(int x, int y, Color oldColor, Color newColor, Layer& layer) {
    if(
        x < 0 || y < 0 || // Base case no.1 & no.2: Index out of bounds
        x >= layer.GetCount().x || y >= layer.GetCount().y || // Base case no.3 & no.4: Index out of bounds
        ColorToInt(layer.GetPixelColor(x, y)) != ColorToInt(oldColor) // Base case no.5: Color at (x,y) doesn't match the input color
    ) {
        return;
    }

    layer.SetPixelColor(x, y, newColor);

    Fill(x - 1, y, oldColor, newColor, layer);
    Fill(x + 1, y, oldColor, newColor, layer);
    Fill(x, y - 1, oldColor, newColor, layer);
    Fill(x, y + 1, oldColor, newColor, layer);
}
