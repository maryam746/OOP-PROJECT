#include "GUI.h"
using namespace std;

GUI::GUI(Cube* cube, CubeSolver* solver, HintSystem* hintSystem)
    : hintSystem(hintSystem) {
    int btnWidth = 120;
    int btnHeight = 40;
    int spacing = 10;
    int screenWidth = GetScreenWidth();

    hintBtn = { (float)spacing, (float)spacing, (float)btnWidth, (float)btnHeight };
    resetBtn = { (float)spacing, (float)(spacing + btnHeight + spacing), (float)btnWidth, (float)btnHeight };

    // Top-right buttons
    scrambleBtn = { (float)(screenWidth - btnWidth - spacing), (float)spacing, (float)btnWidth, (float)btnHeight };
    solveBtn = { (float)(screenWidth - btnWidth - spacing), (float)(spacing + btnHeight + spacing), (float)btnWidth, (float)btnHeight };

    statusMessage = "";
    messageTimer = 0;
}
void GUI::Update(Cube& cube, CubeSolver& solver) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();

        if (CheckCollisionPointRec(mouse, hintBtn)) {
            if (hintSystem->isSolving()) {
                statusMessage = "Still solving...";
            }
            else if (!hintSystem->isSolutionReady()) {
                hintSystem->startSolving();
                statusMessage = "Solving for hint...";
            }
            else {
                string hint = hintSystem->getNextHint();
                statusMessage = "Hint: " + hint;
            }
            messageTimer = 2.0f;
        }
        else if (CheckCollisionPointRec(mouse, resetBtn)) {
            cube = Cube(); // Resets cube
            hintSystem->resetHints(); // Reset hint usage
            statusMessage = "Cube reset and hints cleared!";
            messageTimer = 2.5f;
        }
        else if (CheckCollisionPointRec(mouse, scrambleBtn)) {
            cube.scramble(); // You assume scramble() is implemented
            hintSystem->resetHints();
            statusMessage = "Cube scrambled!";
            messageTimer = 2.5f;
        }
        else if (CheckCollisionPointRec(mouse, solveBtn)) {
            if (hintSystem->isSolving()) {
                statusMessage = "Still solving...";
            }
            else if (!hintSystem->isSolutionReady()) {
                hintSystem->startSolving();
                statusMessage = "Solving the cube...";
            }
            else {
                statusMessage = "Solution is ready!";
            }
            messageTimer = 2.5f;
        }

    }

    // Countdown for message
    if (messageTimer > 0) {
        messageTimer -= GetFrameTime();
        if (messageTimer <= 0) {
            statusMessage = "";
        }
    }
}

void GUI::Draw() {
    DrawButton(hintBtn, "Hint", DARKGREEN);
    DrawButton(resetBtn, "Reset", DARKBLUE);
    DrawButton(scrambleBtn, "Scramble", DARKPURPLE);
    DrawButton(solveBtn, "Solve", DARKBROWN);

    // Draw message at the bottom
    if (!statusMessage.empty()) {
        int fontSize = 20;
        int textWidth = MeasureText(statusMessage.c_str(), fontSize);
        int x = (GetScreenWidth() - textWidth) / 2;
        int y = GetScreenHeight() - 50;
        DrawText(statusMessage.c_str(), x, y, fontSize, RAYWHITE);
    }
}

void GUI::DrawButton(Rectangle rect, const char* text, Color bgColor) {
    DrawRectangleRec(rect, bgColor);
    DrawRectangleLinesEx(rect, 2, WHITE);
    int fontSize = 16;
    int textWidth = MeasureText(text, fontSize);
    DrawText(text,
        rect.x + (rect.width - textWidth) / 2,
        rect.y + (rect.height - fontSize) / 2,
        fontSize,
        WHITE);
}
