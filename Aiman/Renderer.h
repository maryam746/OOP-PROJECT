#ifndef RENDERER_H
#define RENDERER_H

#include "Cube.h"
#include <raylib.h>
#include <array>

class Renderer {
public:
    Renderer();
    ~Renderer();

    void Init();                    // Initialize window and renderer
    void Draw(const Cube& cube);    // Draw the Rubik's Cube based on its current state
    void Close();                   // Close Window and cleanup

private:
    void DrawCubie(int x, int y, int z, const Cube& cube);  // Draw a single small cube (cubie) at position with correct colored faces
    Color ConvertColour(Cube::Colour colour);               // Map Cube::Colour enum to raylib Color

    Camera3D camera;  // Camera for 3D view
};

#endif // RENDERER_H

