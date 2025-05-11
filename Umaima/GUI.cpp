#include "GUI.h"
using namespace std;

GUI::GUI(Cube* cube, CubeSolver* solver, HintSystem* hintSystem)
    : hintSystem(hintSystem) {
    int btnWidth = 200;
    int btnHeight = 60;
    int spacing = 20;
    int screenWidth = GetScreenWidth();

    hintBtn = { (float)spacing, (float)spacing, (float)btnWidth, (float)btnHeight };
    resetBtn = { (float)spacing, (float)(spacing + btnHeight + spacing), (float)btnWidth, (float)btnHeight };

    scrambleBtn = { (float)(screenWidth - btnWidth - spacing), (float)spacing, (float)btnWidth, (float)btnHeight };
    solveBtn = { (float)(screenWidth - btnWidth - spacing), (float)(spacing + btnHeight + spacing), (float)btnWidth, (float)btnHeight };

    statusMessage = "";
    messageTimer = 0;

    animation = {};
    animation.active = false;
}

// Attempt to start rotation animation if no animation currently active
bool GUI::startRotationAnimation(AnimLayerType type, int index, Cube::Rotation direction, Cube::Face face) {
    if (animation.active) return false; // animation in progress, reject new animation

    animation.type = type;
    animation.index = index;
    animation.direction = direction;
    animation.angle = 0.0f;
    animation.speed = 360.0f; // degrees per second, can tune this for speed
    animation.face = face;
    animation.active = true;
    return true;
}

// Apply the actual rotation to cube when animation completes
void GUI::finishRotationAnimation(Cube& cube) {
    switch (animation.type) {
    case AnimLayerType::Row:
        cube.rotateRow(animation.index, animation.direction);
        break;
    case AnimLayerType::Column:
        cube.rotateColumn(animation.index, animation.direction);
        break;
    case AnimLayerType::Face:
        cube.rotateFace(animation.face, animation.direction);
        break;
    default:
        break;
    }
    animation.active = false;
}

// Key press to move mapping helper
// Returns true if animation started successfully, false otherwise
bool handleMoveKeyWithAnim(GUI& gui, Cube& cube, int key, const string& moveName, AnimLayerType animType, int index, Cube::Face face, Cube::Rotation clockwiseDir, Cube::Rotation counterDir) {
    if (IsKeyPressed(key)) {
        Cube::Rotation dir = Cube::Rotation::CLOCKWISE;
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
            dir = Cube::Rotation::COUNTER_CLOCKWISE;
        }

        if (gui.startRotationAnimation(animType, index, dir, face)) {
            return true;
        }
    }
    return false;
}

std::pair<Cube::Face, int> GUI::MapCameraToCubeMove(float yaw) {
    // Normalize yaw to [0, 360)
    float normYaw = fmod((yaw + 360.0f), 360.0f);
    // Determine which face is currently "front"
    if (normYaw < 45 || normYaw >= 315) {
        return { Cube::Face::FRONT, 1 }; // Front face
    }
    else if (normYaw >= 45 && normYaw < 135) {
        return { Cube::Face::RIGHT, 1 }; // Right face
    }
    else if (normYaw >= 135 && normYaw < 225) {
        return { Cube::Face::BACK, 1 }; // Back face
    }
    else {
        return { Cube::Face::LEFT, 1 }; // Left face
    }
}


void GUI::Update(Cube& cube, CubeSolver& solver, float cameraYaw, float cameraPitch) {
    this->cameraYaw = cameraYaw; // Store the camera state
    this->cameraPitch = cameraPitch;

    float deltaTime = GetFrameTime();

    // Update animation if active
    if (animation.active) {
        animation.angle += animation.speed * deltaTime;
        if (animation.angle >= 90.0f) {
            // Animation complete
            finishRotationAnimation(cube);
            messageTimer = 1.0f;
        }
        else {
            // In-progress animation, block user input for moves
            return;
        }
    }

    // Handle buttons as before
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
            cube = Cube(); // Reset cube
            hintSystem->resetHints();
            statusMessage = "Cube reset and hints cleared!";
            messageTimer = 2.5f;
        }
        else if (CheckCollisionPointRec(mouse, scrambleBtn)) {
            cube.scramble();
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
                vector<string> solutionMoves = solver.solve(cube);
                if (solutionMoves.empty()) {
                    statusMessage = "No solution found!";
                }
                else {
                    for (const string& move : solutionMoves) {
                        cube.applyMove(move);
                    }
                    statusMessage = "Cube solved!";
                }
            }
            messageTimer = 2.5f;
        }
    }

    // Now handle key presses for moves with animation
    // Here, we treat face moves as animating the entire face
    // For simplicity, we'll do face moves only here

    if (IsKeyPressed(KEY_R)) {
        Cube::Rotation dir = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT) ? Cube::Rotation::COUNTER_CLOCKWISE : Cube::Rotation::CLOCKWISE;
        if (startRotationAnimation(AnimLayerType::Face, -1, dir, Cube::Face::RIGHT)) {
            statusMessage = "Move Applied: R" + string(dir == Cube::Rotation::COUNTER_CLOCKWISE ? "'" : "");
            messageTimer = 1.5f;
        }
    }
    else if (IsKeyPressed(KEY_L)) {
        Cube::Rotation dir = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT) ? Cube::Rotation::COUNTER_CLOCKWISE : Cube::Rotation::CLOCKWISE;
        if (startRotationAnimation(AnimLayerType::Face, -1, dir, Cube::Face::LEFT)) {
            statusMessage = "Move Applied: L" + string(dir == Cube::Rotation::COUNTER_CLOCKWISE ? "'" : "");
            messageTimer = 1.5f;
        }
    }
    else if (IsKeyPressed(KEY_U)) {
        Cube::Rotation dir = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT) ? Cube::Rotation::COUNTER_CLOCKWISE : Cube::Rotation::CLOCKWISE;
        if (startRotationAnimation(AnimLayerType::Face, -1, dir, Cube::Face::UP)) {
            statusMessage = "Move Applied: U" + string(dir == Cube::Rotation::COUNTER_CLOCKWISE ? "'" : "");
            messageTimer = 1.5f;
        }
    }
    else if (IsKeyPressed(KEY_D)) {
        Cube::Rotation dir = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT) ? Cube::Rotation::COUNTER_CLOCKWISE : Cube::Rotation::CLOCKWISE;
        if (startRotationAnimation(AnimLayerType::Face, -1, dir, Cube::Face::DOWN)) {
            statusMessage = "Move Applied: D" + string(dir == Cube::Rotation::COUNTER_CLOCKWISE ? "'" : "");
            messageTimer = 1.5f;
        }
    }
    else if (IsKeyPressed(KEY_F)) {
        Cube::Rotation dir = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT) ? Cube::Rotation::COUNTER_CLOCKWISE : Cube::Rotation::CLOCKWISE;
        if (startRotationAnimation(AnimLayerType::Face, -1, dir, Cube::Face::FRONT)) {
            statusMessage = "Move Applied: F" + string(dir == Cube::Rotation::COUNTER_CLOCKWISE ? "'" : "");
            messageTimer = 1.5f;
        }
    }
    else if (IsKeyPressed(KEY_B)) {
        Cube::Rotation dir = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT) ? Cube::Rotation::COUNTER_CLOCKWISE : Cube::Rotation::CLOCKWISE;
        if (startRotationAnimation(AnimLayerType::Face, -1, dir, Cube::Face::BACK)) {
            statusMessage = "Move Applied: B" + string(dir == Cube::Rotation::COUNTER_CLOCKWISE ? "'" : "");
            messageTimer = 1.5f;
        }
    }

    // Countdown for message timer
    if (messageTimer > 0) {
        messageTimer -= deltaTime;
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
    int fontSize = 25;
    int textWidth = MeasureText(text, fontSize);
    DrawText(text,
        rect.x + (rect.width - textWidth) / 2,
        rect.y + (rect.height - fontSize) / 2,
        fontSize,
        WHITE);
}

