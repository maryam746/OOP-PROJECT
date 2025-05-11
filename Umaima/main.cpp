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
        gui.Update(cube, solver);
        renderer.Draw(cube);
        gui.Draw();
    }

    renderer.Close();
    return 0;
}
