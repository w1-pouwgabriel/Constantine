#include "GraphicsApp.h"
#include "Scene.h"

GraphicsApp::GraphicsApp(int width, int height)
{
    CreateGLFW(width, height);

    graphics = new VkGraphics(width, height, window);
	
}

GraphicsApp::~GraphicsApp()
{
	delete graphics;	

	glfwDestroyWindow(window);
	glfwTerminate();
}

void GraphicsApp::CreateGLFW(int width, int height)
{
    //initialize glfw
	glfwInit();

	//no default rendering client, we'll hook vulkan up
	//to the window later
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	//resizing breaks the swapchain, we'll disable it for now
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	//GLFWwindow* glfwCreateWindow (int width, int height, const char *title, GLFWmonitor *monitor, GLFWwindow *share)
	if (window = glfwCreateWindow(width, height, "WindowGLFW", nullptr, nullptr)) {
		#if ENABLE_VALIDATION_LAYER
			std::cout << "Successfully made a glfw window, width: " << width << ", height: " << height << '\n';
		#endif
	}
	else {
		#if ENABLE_VALIDATION_LAYER
			std::cout << "GLFW window creation failed\n";
		#endif
	}
}

void GraphicsApp::CalculateFrameRate()
{
	currentTime = glfwGetTime();
	double delta = currentTime - lastTime;

	if (delta >= 1) {
		int framerate{ std::max(1, int(numFrames / delta)) };
		std::stringstream title;
		title << "Running at " << framerate << " fps.";
		glfwSetWindowTitle(window, title.str().c_str());
		lastTime = currentTime;
		numFrames = -1;
		frameTime = float(1000.0 / framerate);
	}

	++numFrames;
}

void GraphicsApp::Run(Scene& scene)
{
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		if(width > 0 && height > 0){
			graphics->Render(scene);
			CalculateFrameRate();
		}
	}
}