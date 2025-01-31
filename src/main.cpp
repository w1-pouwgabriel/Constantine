#include "headers/AssetManager.h"
#include "headers/GraphicsCPU.h"
#include "headers/TriangleMesh.h"

#include <cstdlib>
#include <iostream>

int main()
{
    std::cout << "Hello, World!" << std::endl;

    //Initialize the AssetManager
    AssetManager& assetManager = AssetManager::getInstance();
    tinygltf::Model model = assetManager.loadModel("assets/Cube/Cube.gltf");

    TriangleMesh cube = TriangleMesh(model);

    GraphicsCPU graphics;
    bool result = graphics.initialize(800, 600, "Ray Tracer");
    if (!result) {
        return -1;
    }

    //Load the mesh into the graphics system
    graphics.addMesh(cube);

    graphics.renderLoop();
    graphics.shutdown();

    system("pause");
        
    return 0;
}