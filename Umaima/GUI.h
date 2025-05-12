#pragma once
#include "Cube.h"
#include <queue>
#include <raylib.h>
#include <iostream>
#include <string>

using namespace std;

enum class AnimLayerType {
    None,
    Row,
    Column,
    Face
};

struct RotationAnimation {
    AnimLayerType type = AnimLayerType::None;
    int index = -1; 
    Cube::Rotation direction = Cube::Rotation::CLOCKWISE;
    float angle = 0.0f;   
    float speed = 180.0f; 
    Cube::Face face = Cube::Face::FRONT; 
    bool active = false;
};

class GUI {
public:
    GUI(Cube* cube);
    void Update(Cube& cube); 
    void Draw();

    bool isAnimating() const { return animation.active; }
    const RotationAnimation& getAnimation() const { return animation; }

    bool startRotationAnimation(AnimLayerType type, int index, Cube::Rotation direction, Cube::Face face = Cube::Face::FRONT);

    void finishRotationAnimation(Cube& cube);
    queue<tuple<AnimLayerType, int, Cube::Rotation, Cube::Face>> scrambleQueue;
    bool isScrambling = false;

    void UpdateScrambleAnimation(Cube& cube, float deltaTime);


private:
    Rectangle  resetBtn, scrambleBtn;
    void DrawButton(Rectangle rect, const char* text, Color bgColor);
   
    string statusMessage;
    float messageTimer;
    RotationAnimation animation;

    pair<Cube::Face, int> MapCameraToCubeMove(float yaw);
  
    float scrambleTimer = 0.0f;
    const float scrambleDelay = 0.5f;
};
