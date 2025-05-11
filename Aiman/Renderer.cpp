#include "Renderer.h"
#include <iostream>
#include <cmath>

using namespace std;

Renderer::Renderer() : camera{} {

    camera.position = { 4.0f, 4.0f, 8.0f };  
    camera.target = { 0.0f, 0.0f, 0.0f };  
    camera.up = { 0.0f, 1.0f, 0.0f };       
    camera.fovy = 50.0f;                     
}

Renderer::~Renderer() {
    Close(); 
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


    float sensitivity = 0.3f;


    yaw += mouseDelta.x * sensitivity;
    pitch -= mouseDelta.y * sensitivity;


    pitch = Clamp(pitch, -89.0f, 89.0f);


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


    UpdateCameraOrbit();

    BeginMode3D(camera);


    for (int x = 0; x < 3; ++x) {
        for (int y = 0; y < 3; ++y) {
            for (int z = 0; z < 3; ++z) {

                DrawCubie(x - 1, y - 1, z - 1, cube);
            }
        }
    }

    EndMode3D();

    DrawText("3D Rubik's Cube - By Umaima, Maryam & Aiman", 450, 10, 20, LIGHTGRAY);
    DrawText("Rotate View:Drag mouse", 450, 40, 20, LIGHTGRAY);
    DrawText("Rotate Cube Layers: Use arrow keys", 450, 70, 20, LIGHTGRAY);
	DrawText("*Front Face is the Red Face*", 450, 150, 20, LIGHTGRAY);

    DrawText("Move Controls", 50, 390, 20, RAYWHITE);
    DrawText("u -(U) - Up Clockwise", 30, 425, 20, RAYWHITE);
    DrawText("U - (U') - Up Counterclockwise", 30, 450, 20, RAYWHITE);
    DrawText("d - (D)  - Down Clockwise", 30, 475, 20, RAYWHITE);
    DrawText("D - (D') - Down Counterclockwise", 30, 500, 20, RAYWHITE);
    DrawText("l - (L) - Left Clockwise", 30, 525, 20, RAYWHITE);
    DrawText("L - (L') - Left Counterclockwise", 30, 550, 20, RAYWHITE);


    DrawText("Move Control:", 990, 390, 20, RAYWHITE);
    DrawText("r - (R)  - Right Clockwise", 910, 425, 20, RAYWHITE);
    DrawText("R - (R') - Right Counterclockwise", 910, 450, 20, RAYWHITE);
    DrawText("f - (F) - Front Clockwise", 910, 475, 20, RAYWHITE);
    DrawText("F - (F') - Front Counterclockwise", 910, 500, 20, RAYWHITE);
    DrawText("b - (B) - Back Clockwise", 910, 525, 20, RAYWHITE);
    DrawText("B - (B') - Back Counterclockwise", 910, 550, 20, RAYWHITE);


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
        return GRAY; 
    }
}

void Renderer::DrawCubie(int x, int y, int z, const Cube& cube) {
    const float cubieSize = 0.92f;
    Vector3 position = { (float)x, (float)y, (float)z };


    DrawCube(position, cubieSize, cubieSize, cubieSize, BLACK);

    float faceOffset = cubieSize / 2 + 0.0001f;
    float faceSize = cubieSize * 0.99f;


    if (z == 1) {
        int row = 2 - (y + 1);
        int col = x + 1;
        if (row >= 0 && row < 3 && col >= 0 && col < 3) {
            Color faceColor = ConvertColour(cube.getColour(Cube::Face::FRONT, row, col));
            Vector3 facePos = { position.x, position.y, position.z + faceOffset };
            DrawCube(facePos, faceSize, faceSize, 0.01f, faceColor);
            DrawCubeWires(facePos, faceSize, faceSize, 0.01f, BLACK); 

        }
    }


    if (z == -1) {
        int row = 2 - (y + 1);
        int col = 2 - (x + 1);
        if (row >= 0 && row < 3 && col >= 0 && col < 3) {
            Color faceColor = ConvertColour(cube.getColour(Cube::Face::BACK, row, col));
            Vector3 facePos = { position.x, position.y, position.z - faceOffset };
            DrawCube(facePos, faceSize, faceSize, 0.01f, faceColor);
            DrawCubeWires(facePos, faceSize, faceSize, 0.01f, BLACK); 
        }
    }


    if (x == -1) {
        int row = 2 - (y + 1);
        int col = z + 1;
        if (row >= 0 && row < 3 && col >= 0 && col < 3) {
            Color faceColor = ConvertColour(cube.getColour(Cube::Face::LEFT, row, col));
            Vector3 facePos = { position.x - faceOffset, position.y, position.z };
            DrawCube(facePos, 0.01f, faceSize, faceSize, faceColor);
            DrawCubeWires(facePos, 0.01f, faceSize, faceSize, BLACK); 
        }
    }


    if (x == 1) {
        int row = 2 - (y + 1);
        int col = z + 1;
        if (row >= 0 && row < 3 && col >= 0 && col < 3) {
            Color faceColor = ConvertColour(cube.getColour(Cube::Face::RIGHT, row, col));
            Vector3 facePos = { position.x + faceOffset, position.y, position.z };
            DrawCube(facePos, 0.01f, faceSize, faceSize, faceColor);
            DrawCubeWires(facePos, 0.01f, faceSize, faceSize, BLACK); 
        }
    }


    if (y == 1) {
        int row = 2 - (z + 1);
        int col = x + 1;
        if (row >= 0 && row < 3 && col >= 0 && col < 3) {
            Color faceColor = ConvertColour(cube.getColour(Cube::Face::UP, row, col));
            Vector3 facePos = { position.x, position.y + faceOffset, position.z };
            DrawCube(facePos, faceSize, 0.01f, faceSize, faceColor);
            DrawCubeWires(facePos, faceSize, 0.01f, faceSize, BLACK); 
        }
    }

    if (y == -1) {
        int row = z + 1;
        int col = x + 1;
        if (row >= 0 && row < 3 && col >= 0 && col < 3) {
            Color faceColor = ConvertColour(cube.getColour(Cube::Face::DOWN, row, col));
            Vector3 facePos = { position.x, position.y - faceOffset, position.z };
            DrawCube(facePos, faceSize, 0.01f, faceSize, faceColor);
            DrawCubeWires(facePos, faceSize, 0.001f, faceSize, BLACK);
        }
    }
}
