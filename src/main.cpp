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
    renderer.Init(simulation);

    int simStepsPerFrame = 1;
    while (simulationRunning) {
        for(int i = 0;i < simStepsPerFrame;i++) {
            simulation.Update();
        }
        
        renderer.Render(simulation);
    }

    return 0;
}
