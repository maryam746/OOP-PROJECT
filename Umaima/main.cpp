#include "Cube.h"
#include "Renderer.h"
#include "GUI.h"
#include "CubeSolver.h"
#include <iostream>
using namespace std;

int main() {
    Renderer renderer;
    renderer.Init();

    Cube cube;
    CubeSolver solver(cube);
    HintSystem hintSystem(&cube, &solver);
    GUI gui(&cube, &solver, &hintSystem);

    while (!WindowShouldClose()) {
        // Pass current camera yaw and pitch to GUI update
        gui.Update(cube, solver, renderer.getYaw(), renderer.getPitch());

        renderer.Draw(cube);  // You may want to modify Draw to accept animation if you implement animations

        gui.Draw();
    }



    renderer.Close();
    return 0;
}
