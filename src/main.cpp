#include <cstdlib>
#include <iostream>
#include "headers/GraphicsCPU.h"

int main() {
    
    std::cout << "Hello, World!" << std::endl;
    GraphicsCPU graphics;
    if (!graphics.initialize(800, 600, "Window")) {
        return -1;
    }

    graphics.renderLoop();
    graphics.shutdown();
    
    system("pause");
    return 0;
}