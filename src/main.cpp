
//This is a project where I will be experimenting with opengl in order
//to progress in an exploration of developing a physics engine

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include <glad/glad.h>

#include "Shader.h"
#include "Callbacks.h"
#include "Level.h"
#include "Timer.h"
#include "Vertex.h"
#include "Renderer.h"

#define USE_GPU_ENGINE 0


static Timer tickMaster;


extern "C"
{
	__declspec(dllexport) unsigned long NvOptimusEnablement = USE_GPU_ENGINE;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = USE_GPU_ENGINE;
}


//here is where we get all of the user input
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	

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



	//shader loading example
	Shader s;
	s.loadShaderProgramFromFile(RESOURCES_PATH "vertex.vert", RESOURCES_PATH "fragment.frag");
	s.bind();

	int startTick = tickMaster.getTicks();
	int loopCount = 0;

	std::vector<Vertex> mesh;

		mesh.emplace_back(-0.5f, 0.5f, 0.0f);
		mesh.emplace_back(-0.5f, -0.5f, 0.0f);
		mesh.emplace_back(0.5f, -0.5f, 0.0f);
		mesh.emplace_back(0.5f, 0.5f, 0.0f); 

	std::vector<int> elem = { 0,1,2,0,2,3 };

	Renderer Renderer;


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
			

			uint32_t vao = Renderer.uploadMesh(mesh, elem);
			Renderer.drawMesh(vao, elem.size());
			Renderer.unloadMesh(vao);
			
			glfwSwapBuffers(window);
			glfwPollEvents();
			

		}
		
	}
	return 0;
}
