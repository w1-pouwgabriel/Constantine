#pragma once

#include "GraphicsApp.h"
#include "Scene.h"
#include <memory>

class Engine
{
public:
	Engine();
	~Engine();

	void Run();

private:
	GraphicsApp* graphics;
	Scene scene;
};