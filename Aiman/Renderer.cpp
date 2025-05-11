#include "Renderer.h"
#include <iostream>
#include <cmath>

using namespace std;

Renderer::Renderer() : camera{} {
    // Initialize the camera with default values
    camera.position = { 4.0f, 4.0f, 8.0f };  // Camera position
    camera.target = { 0.0f, 0.0f, 0.0f };   // Look at origin
    camera.up = { 0.0f, 1.0f, 0.0f };       // Up vector
    camera.fovy = 50.0f;                     // Field of view
}

Renderer::~Renderer() {
    // Cleanup if necessary
    Close(); // Ensure the window is closed properly
}

void Renderer::Init() {
    InitWindow(1280, 950, "3D Rubik's Cube");
    windowInitialized = true;

                    
    SetTargetFPS(60);
}

void Renderer::Close() {
    if (windowInitialized) {
        CloseWindow();
        windowInitialized = false;
    }
}

void Renderer::UpdateCameraOrbit() {
    Vector2 mouseDelta = GetMouseDelta();

    // Sensitivity
    float sensitivity = 0.3f;

    // Update yaw and pitch
    yaw += mouseDelta.x * sensitivity;
    pitch -= mouseDelta.y * sensitivity;

    // Clamp pitch to prevent flipping
    pitch = Clamp(pitch, -89.0f, 89.0f);

    // Convert spherical to Cartesian coordinates
    float radYaw = DEG2RAD * yaw;
    float radPitch = DEG2RAD * pitch;

    camera.position.x = cameraDistance * cosf(radPitch) * sinf(radYaw);
    camera.position.y = cameraDistance * sinf(radPitch);
    camera.position.z = cameraDistance * cosf(radPitch) * cosf(radYaw);

    camera.target = target;
    camera.up = { 0.0f, 1.0f, 0.0f };
}


float Renderer::Clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}


void Renderer::Draw(const Cube& cube) {

    BeginDrawing();
    ClearBackground(BLACK);

    // Update camera to handle input
    UpdateCameraOrbit();

    BeginMode3D(camera);

    // Draw all 27 cubies in 3x3x3 grid, centered at origin (-1, 0, 1)
    for (int x = 0; x < 3; ++x) {
        for (int y = 0; y < 3; ++y) {
            for (int z = 0; z < 3; ++z) {
                // Adjust the coordinates to center the cube at the origin
                DrawCubie(x - 1, y - 1, z - 1, cube);
            }
        }
    }

    EndMode3D();

    DrawText("3D Rubik's Cube - By Umaima, Maryam & Aiman", 450, 10, 20, LIGHTGRAY);
    DrawText("Rotate View:Drag mouse", 450, 40, 20, LIGHTGRAY);
    DrawText("Rotate Cube Layers: Use arrow keys", 450, 70, 20, LIGHTGRAY);

    EndDrawing();
}

Color Renderer::ConvertColour(Cube::Colour colour) {
    switch (colour) {
    case Cube::Colour::white:  return WHITE;
    case Cube::Colour::yellow: return YELLOW;
    case Cube::Colour::red:    return RED;
    case Cube::Colour::orange: return ORANGE;
    case Cube::Colour::blue:   return BLUE;
    case Cube::Colour::green:  return GREEN;
    default:
        cerr << "Unknown colour enum value: " << static_cast<int>(colour) << endl;
        return GRAY; // return a neutral color instead of calling ConvertColour again
    }
}

void Renderer::DrawCubie(int x, int y, int z, const Cube& cube) {
    const float cubieSize = 0.92f;
    Vector3 position = { (float)x, (float)y, (float)z };

    // Draw main black cube
    DrawCube(position, cubieSize, cubieSize, cubieSize, BLACK);

    float faceOffset = cubieSize / 2 + 0.0001f;
    float faceSize = cubieSize * 0.99f;

    // Front face (z == 1)
    if (z == 1) {
        int row = 2 - (y + 1);
        int col = x + 1;
        if (row >= 0 && row < 3 && col >= 0 && col < 3) {
            Color faceColor = ConvertColour(cube.getColour(Cube::Face::FRONT, row, col));
            Vector3 facePos = { position.x, position.y, position.z + faceOffset };
            DrawCube(facePos, faceSize, faceSize, 0.01f, faceColor);
            DrawCubeWires(facePos, faceSize, faceSize, 0.01f, BLACK); // Ensure dimensions match
        }
    }

    // Back face (z == -1)
    if (z == -1) {
        int row = 2 - (y + 1);
        int col = 2 - (x + 1);
        if (row >= 0 && row < 3 && col >= 0 && col < 3) {
            Color faceColor = ConvertColour(cube.getColour(Cube::Face::BACK, row, col));
            Vector3 facePos = { position.x, position.y, position.z - faceOffset };
            DrawCube(facePos, faceSize, faceSize, 0.01f, faceColor);
            DrawCubeWires(facePos, faceSize, faceSize, 0.01f, BLACK); // Ensure dimensions match
        }
    }

    // Left face (x == -1)
    if (x == -1) {
        int row = 2 - (y + 1);
        int col = z + 1;
        if (row >= 0 && row < 3 && col >= 0 && col < 3) {
            Color faceColor = ConvertColour(cube.getColour(Cube::Face::LEFT, row, col));
            Vector3 facePos = { position.x - faceOffset, position.y, position.z };
            DrawCube(facePos, 0.01f, faceSize, faceSize, faceColor);
            DrawCubeWires(facePos, 0.01f, faceSize, faceSize, BLACK); // Ensure dimensions match
        }
    }

    // Right face (x == 1)
    if (x == 1) {
        int row = 2 - (y + 1);
        int col = z + 1;
        if (row >= 0 && row < 3 && col >= 0 && col < 3) {
            Color faceColor = ConvertColour(cube.getColour(Cube::Face::RIGHT, row, col));
            Vector3 facePos = { position.x + faceOffset, position.y, position.z };
            DrawCube(facePos, 0.01f, faceSize, faceSize, faceColor);
            DrawCubeWires(facePos, 0.01f, faceSize, faceSize, BLACK); // Ensure dimensions match
        }
    }

    // Up face (y == 1)
    if (y == 1) {
        int row = 2 - (z + 1);
        int col = x + 1;
        if (row >= 0 && row < 3 && col >= 0 && col < 3) {
            Color faceColor = ConvertColour(cube.getColour(Cube::Face::UP, row, col));
            Vector3 facePos = { position.x, position.y + faceOffset, position.z };
            DrawCube(facePos, faceSize, 0.01f, faceSize, faceColor);
            DrawCubeWires(facePos, faceSize, 0.01f, faceSize, BLACK); // Ensure dimensions match
        }
    }

    // Down face (y == -1)
    if (y == -1) {
        int row = z + 1;
        int col = x + 1;
        if (row >= 0 && row < 3 && col >= 0 && col < 3) {
            Color faceColor = ConvertColour(cube.getColour(Cube::Face::DOWN, row, col));
            Vector3 facePos = { position.x, position.y - faceOffset, position.z };
            DrawCube(facePos, faceSize, 0.01f, faceSize, faceColor);
            DrawCubeWires(facePos, faceSize, 0.001f, faceSize, BLACK); // Ensure dimensions match
        }
    }
}

