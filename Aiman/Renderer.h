#pragma once

#include "Cube.h"
#include <raylib.h>
#include <array>

#include <iostream>
using namespace std;

class Renderer {
public:
    Renderer();
    ~Renderer();

    void Init();                    
    void Draw(const Cube& cube);    
    void Close();                   
    void UpdateCameraOrbit(); 
    float Clamp(float value, float min, float max);
    float yaw = 0.0f;
    float pitch = 0.0f;
    float cameraDistance = 10.0f; 
    Vector3 target = { 0.0f, 0.0f, 0.0f }; 
    float getYaw() const { return yaw; }
    float getPitch() const { return pitch; }



private:
    void DrawCubie(int x, int y, int z, const Cube& cube); 
    Color ConvertColour(Cube::Colour colour);              

    Camera3D camera;  
    bool windowInitialized = false;  
};
