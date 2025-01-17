#include <iostream>
#include "headers/GraphicsGL.h"

int main() {
    
    std::cout << "Hello, World!" << std::endl;
    GraphicsGL graphics;
    if (!graphics.initialize(800, 600, "Window")) {
        return -1;
    }

    graphics.renderLoop();
    graphics.shutdown();
    
    return 0;
}