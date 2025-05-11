#include "Cube.h"
#include "Renderer.h"
#include "GUI.h"
#include <iostream>
using namespace std;

int main() {
    Renderer renderer;
    renderer.Init();

    Cube cube;


    GUI gui(&cube);

    while (!WindowShouldClose()) {

        gui.Update(cube);

        renderer.Draw(cube);  

        gui.Draw();
    }



    renderer.Close();
    return 0;
}
