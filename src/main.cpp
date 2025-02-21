#include "headers/AssetManager.h"
#include "headers/TriangleMesh.h"
#include "headers/GraphicsCPU.h"

#include <cstdlib>
#include <iostream>

int main()
{
    //Singelton
    AssetManager& assetManager = AssetManager::getInstance();

    TriangleMesh scene;
    scene.loadGLTF(assetManager.loadModel("assets/Cube/Cube.gltf"));

    GraphicsCPU graphics;
    bool result = graphics.initialize(800, 600, "Ray Tracer");
    if (!result) {
        return -1;
    }

    //Load the mesh into the graphics system
    graphics.addMesh(scene);

    graphics.renderLoop();
    graphics.shutdown();

    system("pause");
    return 0;
}