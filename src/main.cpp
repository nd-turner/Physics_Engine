
//This is a project where I will be experimenting with opengl in order
//to progress in an exploration of developing a physics engine

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>

#include <iostream>
#include <vector>

#include <glad/glad.h>

#include "Shader.h"
#include "Callbacks.h"
#include "Level.h"
#include "Timer.h"
#include "Vertex.h"
#include "Renderer.h"
#include "Geometry.h"

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
	if (!glfwInit()) {return -1;}
		
#pragma region report opengl errors to std
	//this code enables GLFW debugging
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#pragma endregion


	GLFWwindow *window = window = glfwCreateWindow(640, 480, "Sim_Window", NULL, NULL);
	if (!window){glfwTerminate();return -1;}
	
	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glClearColor(.0, .3, .6, 0);
	glfwSwapInterval(1);

	Shader s;
	s.loadShaderProgramFromFile(RESOURCES_PATH "vertex.vert", RESOURCES_PATH "fragment.frag");
	s.bind();

	int startTick = tickMaster.getTicks();
	int loopCount = 0;

	Geometry geo;

	std::vector<Vertex> mesh;
	float pos[3] = {0,0,0};
	mesh = geo.generateCircleMesh(.1,  pos, 1000);

	std::vector<int> elem;
	elem = geo.generateCircleElem(mesh);

	Renderer Renderer;



	//texture

	//int Textwidth, Textheight, TextChannels;
	//unsigned char* image_data = stbi_load("wood.png", &Textwidth, &Textheight, &TextChannels, STBI_rgb);

	//GLuint texture;
	//glGenTextures(1, &texture);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture);

	////scaling
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	////repeating
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Textwidth, Textheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	//glGenerateMipmap(GL_TEXTURE_2D);


	//stbi_image_free(image_data);
	//glBindTexture(GL_TEXTURE_2D, 0);


	while (!glfwWindowShouldClose(window))
	{
		
		int width = 0, height = 0;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);

		tickMaster.tick();
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
	//glDeleteTextures(1, &texture);
}
