#include "headers/AssetManager.h"
#include "headers/GraphicsCPU.h"
#include "headers/primitive/TriangleMesh.h"

#include <cstdlib>
#include <iostream>

int main()
{
    std::cout << "Hello, World!" << std::endl;

    //Initialize the AssetManager
    AssetManager& assetManager = AssetManager::getInstance();

    TriangleMesh mesh;
    mesh.loadGLTF(*assetManager.loadModel("assets/Cube/Cube.gltf"));

    //Initialize the SceneManager
    // SceneManager& sceneManager = SceneManager::getInstance();
    // sceneManager.addScene("main", std::make_shared<CircleScene>());

    GraphicsCPU graphics;
    bool result = graphics.initialize(800, 600, "Ray Tracer");
    if (!result) {
        return -1;
    }

    //Load the mesh into the graphics system
    graphics.addMesh(mesh);

    graphics.renderLoop();
    graphics.shutdown();

    system("pause");
        
    return 0;
}