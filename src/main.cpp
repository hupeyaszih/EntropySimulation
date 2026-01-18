#include <iostream>
#include "core/simulation.h"
int main() {
    bool simulationRunning = true;
    Simulation simulation;
    simulation.Init();
    /*
    while (simulationRunning) {
        simulation.Update();
    }*/

    std::cout << "Hello matrix!" << std::endl;
    return 0;
}
