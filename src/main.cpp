#include <iostream>
#include "headers/GraphicsGL.h"

int main() {
    
    std::cout << "Hello, World!" << std::endl;
    GraphicsGL graphics;
    graphics.initialize(600, 480, "Path tracer");
    graphics.renderLoop();
    
    return 0;
}