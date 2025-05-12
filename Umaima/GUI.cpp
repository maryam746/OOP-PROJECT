#include "GUI.h"
#include <iostream>
using namespace std;

vector<string> scrambleMoves;

GUI::GUI(Cube* cube) {
    int btnWidth = 200;
    int btnHeight = 60;
    int spacing = 20;
    int screenWidth = GetScreenWidth();


    resetBtn = { (float)spacing, (float)spacing, (float)btnWidth, (float)btnHeight };
    scrambleBtn = { (float)(screenWidth - btnWidth - spacing), (float)spacing, (float)btnWidth, (float)btnHeight };

    statusMessage = "";
    messageTimer = 0;

    animation = {};
    animation.active = false;
}


bool GUI::startRotationAnimation(AnimLayerType type, int index, Cube::Rotation direction, Cube::Face face) {
    if (animation.active) return false; 


    animation.type = type;
    animation.index = index;
    animation.direction = direction;
    animation.angle = 0.0f;
    animation.speed = 360.0f; 

    animation.face = face;
    animation.active = true;
    return true;
}


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


static bool handleMoveKeyWithAnim(GUI& gui, Cube& cube, int key, const string& moveName, AnimLayerType animType, int index, Cube::Face face, Cube::Rotation clockwiseDir, Cube::Rotation counterDir) {
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

pair<Cube::Face, int> GUI::MapCameraToCubeMove(float yaw) {
   
    float normYaw = fmod((yaw + 360.0f), 360.0f);

    if (normYaw < 45 || normYaw >= 315) {
        return { Cube::Face::FRONT, 1 }; 
    }
    else if (normYaw >= 45 && normYaw < 135) {
        return { Cube::Face::RIGHT, 1 }; 
    }
    else if (normYaw >= 135 && normYaw < 225) {
        return { Cube::Face::BACK, 1 };
    }
    else {
        return { Cube::Face::LEFT, 1 };
    }
}


void GUI::Update(Cube& cube) {
    float deltaTime = GetFrameTime();


    if (animation.active) {
        animation.angle += animation.speed * deltaTime;
        if (animation.angle >= 90.0f) {
       
            finishRotationAnimation(cube);
            messageTimer = 1.0f;
            animation.active = false;
        }
        return; 
    }


    if (isScrambling && !animation.active) {
        scrambleTimer += deltaTime;
        if (scrambleTimer >= scrambleDelay) {
            if (!scrambleQueue.empty()) {
                auto [type, index, dir, face] = scrambleQueue.front();
                scrambleQueue.pop();
                if (startRotationAnimation(type, index, dir, face)) {
                    scrambleTimer = 0.0f;
                }
            }
            else {
                isScrambling = false;
                statusMessage = "Cube scrambled!";
                messageTimer = 2.5f;
            }
        }
    }


    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();

        if (CheckCollisionPointRec(mouse, resetBtn)) {
            cube = Cube(); 
            scrambleQueue = {};
            isScrambling = false;
            animation.active = false;
            statusMessage = "Cube reset!";
            messageTimer = 2.5f;
        }
        else if (CheckCollisionPointRec(mouse, scrambleBtn)) {
            if (!animation.active && !isScrambling) {
                cube.startScrambleSequence();
                isScrambling = true;
                statusMessage = "Scrambling...";
                messageTimer = 2.5f;
                scrambleTimer = scrambleDelay; 


                while (cube.isScrambling()) {
                    string move = cube.getNextScrambleMove();
                    if (!move.empty()) {
                        Cube::Rotation dir = move.back() == '\'' ?
                            Cube::Rotation::COUNTER_CLOCKWISE :
                            Cube::Rotation::CLOCKWISE;
                        string faceStr = move.substr(0, 1);

                        Cube::Face face;
                        if (faceStr == "R") face = Cube::Face::RIGHT;
                        else if (faceStr == "L") face = Cube::Face::LEFT;
                        else if (faceStr == "U") face = Cube::Face::UP;
                        else if (faceStr == "D") face = Cube::Face::DOWN;
                        else if (faceStr == "F") face = Cube::Face::FRONT;
                        else if (faceStr == "B") face = Cube::Face::BACK;
                        else continue;

                        scrambleQueue.push({ AnimLayerType::Face, -1, dir, face });
                    }
                }
            }
        }
    }


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


    if (messageTimer > 0) {
        messageTimer -= deltaTime;
        if (messageTimer <= 0) {
            statusMessage = "";
        }
    }
}

void GUI::Draw() {

    DrawButton(resetBtn, "Reset", DARKBLUE);
    DrawButton(scrambleBtn, "Scramble", DARKPURPLE);


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

void GUI::UpdateScrambleAnimation(Cube& cube, float deltaTime) {
    if (!isScrambling) return;

    scrambleTimer += deltaTime;
    if (scrambleTimer >= scrambleDelay && !animation.active) {
        if (!scrambleQueue.empty()) {
            auto [type, index, dir, face] = scrambleQueue.front();
            scrambleQueue.pop();
            startRotationAnimation(type, index, dir, face);
            scrambleTimer = 0.0f; 
        }
        else {
            isScrambling = false;
            statusMessage = "Cube scrambled!";
            messageTimer = 2.5f;
        }
    }
}

