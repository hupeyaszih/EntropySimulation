#include <iostream>
#include "core/simulation.h"
#include "renderer/renderer.h"
int main() {
    //Init Simulation
    bool simulationRunning = true;
    Simulation simulation;
    simulation.Init();

    //Init Renderer
    Renderer renderer;
    renderer.Init();

    /*
    while (simulationRunning) {
        simulation.Update();
    }*/

    std::cout << "Hello matrix!" << std::endl;
    return 0;
}
