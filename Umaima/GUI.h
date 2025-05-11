#pragma once
#include "Cube.h"
#include "CubeSolver.h"
#include "Hint.h"
#include <raylib.h>
#include <iostream>
#include <string>
using namespace std;

class GUI {
public:
    GUI(Cube* cube, CubeSolver* solver, HintSystem* hintSystem);
    void Update(Cube& cube, CubeSolver& solver);
    void Draw();

private:
    Rectangle hintBtn, resetBtn, scrambleBtn, solveBtn;
    void DrawButton(Rectangle rect, const char* text, Color bgColor);
    HintSystem* hintSystem;
    string statusMessage;
    float messageTimer;
    bool isDragging = false;
    Vector2 dragStart = { 0, 0 };
    Vector2 dragEnd = { 0, 0 };
};
