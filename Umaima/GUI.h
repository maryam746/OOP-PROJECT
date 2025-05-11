#pragma once
#include "Cube.h"
#include "CubeSolver.h"
#include "Hint.h"
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
    int index = -1; // row or column index or face
    Cube::Rotation direction = Cube::Rotation::CLOCKWISE;
    float angle = 0.0f;    // current angle of rotation in degrees
    float speed = 180.0f;  // degrees per second
    Cube::Face face = Cube::Face::FRONT; // for face rotations if needed
    bool active = false;
};

class GUI {
public:
    GUI(Cube* cube, CubeSolver* solver, HintSystem* hintSystem);
    void Update(Cube& cube, CubeSolver& solver);
    void Draw();

    // Provide animation info getters to Renderer
    bool isAnimating() const { return animation.active; }
    const RotationAnimation& getAnimation() const { return animation; }

    // Start an animation of cube rotation, returns false if animation already running
    bool startRotationAnimation(AnimLayerType type, int index, Cube::Rotation direction, Cube::Face face = Cube::Face::FRONT);

    // Apply the final rotation to the cube once animation ends
    void finishRotationAnimation(Cube& cube);


private:
    Rectangle hintBtn, resetBtn, scrambleBtn, solveBtn;
    void DrawButton(Rectangle rect, const char* text, Color bgColor);
    HintSystem* hintSystem;
    string statusMessage;
    float messageTimer;
    RotationAnimation animation;

    
};
