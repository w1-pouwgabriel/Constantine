#include "Engine.h"

#include "GraphicsApp.h"
#include "Scene.h"

Engine::Engine() 
	: scene()
{
	graphics = new GraphicsApp(640, 480);
}

void Engine::Run()
{
	graphics->Run(scene);
}

Engine::~Engine()
{
	delete graphics;
}
