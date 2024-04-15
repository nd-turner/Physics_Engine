#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <openglDebug.h>
#include <demoShader.h>
#include <iostream>
#include <vector>

#include "Callbacks.h"
#include "Timer.h"
#include "Vertex.h"
#include "openGLDraw.h"


void render(GLFWwindow *window);


static timer tickMaster;
#define USE_GPU_ENGINE 0

extern "C"
{
	__declspec(dllexport) unsigned long NvOptimusEnablement = USE_GPU_ENGINE;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = USE_GPU_ENGINE;
}


//this function closes the window when you press escape
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

}




//render code here this will be called with a parameter of a shape to render that we will implement with time
void render(GLFWwindow *window) {

	

}

int main(void)
{
	glfwSetErrorCallback(glfw_Error_Callback);
	// if glfw does not initalize then exit main
	if (!glfwInit()) {
		return -1;
	}
		
#pragma region report opengl errors to std
	//this code enables GLFW debugging
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#pragma endregion


	GLFWwindow *window = window = glfwCreateWindow(640, 480, "Sim_Window", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	
	glfwSetKeyCallback(window, key_callback);	//link the key_callback function to the current window
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glClearColor(.0, .3, .6, 0);	//set window background
	glfwSwapInterval(1);

#pragma region report opengl errors to std
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(glDebugOutput, 0);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#pragma endregion

	//shader loading example
	Shader s;
	s.loadShaderProgramFromFile(RESOURCES_PATH "vertex.vert", RESOURCES_PATH "fragment.frag");
	s.bind();

	int startTick = tickMaster.getTicks();
	int loopCount = 0;

	std::vector<Vertex> mesh;
	mesh.emplace_back(0, .5, 0);
	mesh.emplace_back(-.5, 0, 0);
	mesh.emplace_back(.5, 0, 0);

	std::vector<int> elem = { 0,1,2 };

	while (!glfwWindowShouldClose(window))
	{
		
		int width = 0, height = 0;
		glfwGetFramebufferSize(window, &width, &height);	//defines the portion of the window where the rendering takes place
		glViewport(0, 0, width, height);	//starts in lower left corner and goes to upper right corner
		glClear(GL_COLOR_BUFFER_BIT);

		tickMaster.tick();	//setup game loop
		int currentTick = tickMaster.getTicks();
	
		
		if (currentTick - startTick >= 1) {		//game loop
			startTick = currentTick;
			loopCount++;
			std::cout << loopCount;



			draw Canvas(mesh, elem);
			uint32_t vao = Canvas.uploadMesh(mesh, elem);
			Canvas.drawMesh(vao, elem.size());


			glfwSwapBuffers(window);
			glfwPollEvents();

		}
	
	}
	return 0;
}
