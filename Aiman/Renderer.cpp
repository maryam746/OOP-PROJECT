#include "Renderer.h"
#include <array>

Renderer::Renderer() {
    // Constructor
}

Renderer::~Renderer() {
    // Destructor
}

void Renderer::Init() {
    InitWindow(800, 600, "3D Rubik's Cube");
    // Initialize camera looking at origin from positive diagonal
    camera.position = {4.0f, 4.0f, 4.0f};  // Camera position
    camera.target = {0.0f, 0.0f, 0.0f};     // Look at origin
    camera.up = {0.0f, 1.0f, 0.0f};         // Up vector
    camera.fovy = 45.0f;                     // Field of view
    // No camera.type assignment due to Camera3D lacking type member

    SetTargetFPS(60);
}

void Renderer::Draw(const Cube& cube) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    BeginMode3D(camera);

    // Draw coordinate axis for reference (optional)
    DrawGrid(5, 1.0f);

    // Draw all 27 cubies in 3x3x3 grid, centered at origin (-1,0,1)
    for (int x = 0; x < 3; ++x) {
        for (int y = 0; y < 3; ++y) {
            for (int z = 0; z < 3; ++z) {
                DrawCubie(x - 1, y - 1, z - 1, cube);
            }
        }
    }

    EndMode3D();

    DrawText("3D Rubik's Cube - Use mouse to look around", 10, 10, 20, DARKGRAY);
    EndDrawing();
}

void Renderer::Close() {
    CloseWindow();
}

Color Renderer::ConvertColour(Cube::Colour colour) {
    switch (colour) {
        case Cube::Colour::red: return RED;
        case Cube::Colour::orange: return ORANGE;
        case Cube::Colour::green: return GREEN;
        case Cube::Colour::blue: return BLUE;
        case Cube::Colour::white: return WHITE;
        case Cube::Colour::yellow: return YELLOW;
        case Cube::Colour::black: return BLACK;
        default: return GRAY;
    }
}

void Renderer::DrawCubie(int x, int y, int z, const Cube& cube) {
    const float cubieSize = 0.9f;
    Vector3 position = {(float)x, (float)y, (float)z};

    // Draw main black cube
    DrawCube(position, cubieSize, cubieSize, cubieSize, BLACK);

    float faceOffset = cubieSize / 2 + 0.01f;
    float faceSize = cubieSize * 0.95f;

    // Front face (z == 1)
    if (z == 1) {
        Color faceColor = ConvertColour(cube.getColour(Cube::Face::FRONT, 2 - y, x));
        Vector3 facePos = {position.x, position.y, position.z + faceOffset};
        DrawCube(facePos, faceSize, faceSize, 0.01f, faceColor);
        DrawCubeWires(facePos, faceSize, faceSize, 0.01f, BLACK);
    }

    // Back face (z == -1)
    if (z == -1) {
        Color faceColor = ConvertColour(cube.getColour(Cube::Face::BACK, 2 - y, 2 - x));
        Vector3 facePos = {position.x, position.y, position.z - faceOffset};
        DrawCube(facePos, faceSize, faceSize, 0.01f, faceColor);
        DrawCubeWires(facePos, faceSize, faceSize, 0.01f, BLACK);
    }

    // Left face (x == -1)
    if (x == -1) {
        Color faceColor = ConvertColour(cube.getColour(Cube::Face::LEFT, 2 - y, z));
        Vector3 facePos = {position.x - faceOffset, position.y, position.z};
        DrawCube(facePos, 0.01f, faceSize, faceSize, faceColor);
        DrawCubeWires(facePos, 0.01f, faceSize, faceSize, BLACK);
    }

    // Right face (x == 1)
    if (x == 1) {
        Color faceColor = ConvertColour(cube.getColour(Cube::Face::RIGHT, 2 - y, 2 - z));
        Vector3 facePos = {position.x + faceOffset, position.y, position.z};
        DrawCube(facePos, 0.01f, faceSize, faceSize, faceColor);
        DrawCubeWires(facePos, 0.01f, faceSize, faceSize, BLACK);
    }

    // Up face (y == 1)
    if (y == 1) {
        Color faceColor = ConvertColour(cube.getColour(Cube::Face::UP, 2 - z, x));
        Vector3 facePos = {position.x, position.y + faceOffset, position.z};
        DrawCube(facePos, faceSize, 0.01f, faceSize, faceColor);
        DrawCubeWires(facePos, faceSize, 0.01f, faceSize, BLACK);
    }

    // Down face (y == -1)
    if (y == -1) {
        Color faceColor = ConvertColour(cube.getColour(Cube::Face::DOWN, z, x));
        Vector3 facePos = {position.x, position.y - faceOffset, position.z};
        DrawCube(facePos, faceSize, 0.01f, faceSize, faceColor);
        DrawCubeWires(facePos, faceSize, 0.01f, faceSize, BLACK);
    }
}

