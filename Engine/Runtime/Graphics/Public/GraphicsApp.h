#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "Utils.h"
#include "VkGraphics.h"

class GraphicsApp {
public:
	GraphicsApp(int width, int height);
	~GraphicsApp();
	
	void Run(Scene& scene);

private:
	VkGraphics* graphics;
	GLFWwindow* window;

	double lastTime, currentTime;
	int numFrames;
	float frameTime;

	void CreateGLFW(int width, int height);
	void CalculateFrameRate();
};