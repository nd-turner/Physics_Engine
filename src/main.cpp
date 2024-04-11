#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <openglDebug.h>
#include <demoShader.h>
#include <iostream>

#include "Callbacks.h"
#include "Timer.h"




void render();

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
void render() {

	
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


	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //you might want to do this when testing the game for shipping


	GLFWwindow *window = window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}


	//link the key_callback function to the current window
	glfwSetKeyCallback(window, key_callback);

	//specify that window will be taking next glfw commands
	glfwMakeContextCurrent(window);

	//this initalizes the GLAD library which is integrated with openGL
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	//enable v-sync (not quite sure what that is)
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

	while (!glfwWindowShouldClose(window))
	{
		
		int width = 0, height = 0;

		//gets the size in pixels to allow the rendering to be adaptive to resizing
		glfwGetFramebufferSize(window, &width, &height);
		//defines the portion of the window where the rendering takes place
		glViewport(0, 0, width, height);	//starts in lower left corner and goes to upper right corner

		glClear(GL_COLOR_BUFFER_BIT);

		tickMaster.tick();
		int ticker = tickMaster.getTicks();
		//so ticker is an integer that counts up at a rate say once every second, I need to run code on every tick

		if (tickMaster.getTickEdge()) {
			std::cout << "edgeFound" << "\n";
		}
		
		render();
		//actually render the triangle
		glBegin(GL_TRIANGLES);
			glVertex2f(0,.5);
			glVertex2f(-.5,0);
			glVertex2f(.5,0);
		glEnd();

		glfwSwapBuffers(window); //presents current image to the screen for the frame
		//printf("1st frame \n");
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_TRIANGLES);
			glVertex2f(0, 1);
			glVertex2f(-1, 0);
			glVertex2f(1, 0);
		glEnd();

		glfwSwapBuffers(window); //I think this presents the frame

		
		//printf("2nd frame \n");
	}

	//there is no need to call the clear function for the libraries since the os will do that for us.
	//by calling this functions we are just wasting time.
	//glfwDestroyWindow(window);
	//glfwTerminate();
	return 0;
}
