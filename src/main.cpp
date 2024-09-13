
//This is a project where I will be experimenting with opengl in order
//to progress in an exploration of developing a physics engine

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>
#include <random>

#include <cmath>
#include <iostream>
#include <vector>

#include <glad/glad.h>

#include "Shader.h"
#include "Callbacks.h"
#include "Level.h"
#include "Timer.h"
#include "Vertex.h"
#include "Renderer.h"
#include "Object.h"

#define USE_GPU_ENGINE 0


static Timer tickMaster;

// Function to generate a random float between min and max
float generateRandomFloat(float min, float max) {
	std::random_device rd;  // Obtain a random number from hardware
	std::mt19937 gen(rd()); // Seed the generator
	std::uniform_real_distribution<> dis(min, max); // Define the range

	return dis(gen);
}

// Function to generate a random float array of size 3
void generateRandomFloatArray(float arr[3], float min, float max) {
	for (int i = 0; i < 3; ++i) {
		arr[i] = generateRandomFloat(min, max);
	}
}


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


	GLFWwindow *window = window = glfwCreateWindow(640, 640, "Sim_Window", NULL, NULL);
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

	int numCirc = 1;
	
	float InitPos[3] = { 0.0f,0.7f,0.0f };
	float InitVel[3] = { -0.01f,0.01f,0.0f };
	float Circ1Rad = 0.1f;


	
	std::vector<Object> GameObjects;


	//generates 5 randomly initialized circles and stores them in GameObjects vector
	for (int i = 0; i < numCirc; i++) {
		float InitPos[3] = { 0.0f,0.7f,0.0f };
		float InitVel[3] = { -0.01f,0.01f,0.0f };
		
		/*generateRandomFloatArray(InitPos, -1, 1);
		generateRandomFloatArray(InitVel, -1, 1);
		float RndCircRad = generateRandomFloat(-1, 1);*/
		
		float RndCircRad = 0.5f;

		Object circ(InitPos, InitVel);
		circ.setRad(RndCircRad);

		GameObjects.emplace_back(circ);
	}
	
	
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
		
		//mesh and elem matrix for the entire screen
		std::vector<Vertex> mesh;
		std::vector<int> elem;
		
		//generate all of the game objects

		for (int i = 0; i < GameObjects.size(); i++) {

			std::vector<Vertex> TempMesh = GameObjects[i].generateCircleMesh();
			std::vector<int> TempElem = GameObjects[i].generateCircleElem(TempMesh);

			// Concatenate vertices and elements from circles and combine into single mesh
			mesh.insert(TempMesh.end(), TempMesh.begin(), TempMesh.end());
			elem.insert(elem.end(), TempElem.begin(), TempElem.end());

		}
		// Concatenate elements from both circles into a single set of elements

		if (currentTick - startTick >= 1) {		//game loop
			startTick = currentTick;
			loopCount++;

			//go through all game objects
			for (int i = 0; i < GameObjects.size(); i++) {
				const float* pos = GameObjects[i].getPosition();
				const float* vel = GameObjects[i].getVelocity();

				float newPosition[3];

				for (int j = 0; j < 3; j++) {
					newPosition[j] = pos[j] + vel[j];
				}

				GameObjects[i].updatePosition(newPosition);

				// Check for collisions
				if (GameObjects[i].isColliding()) {
					GameObjects[i].handleWallCollision();
				}
			}


			uint32_t vao = Renderer.uploadMesh(mesh, elem);
			Renderer.drawMesh(vao, elem.size());
			Renderer.unloadMesh(vao);

			glfwSwapBuffers(window);
			glfwPollEvents();


		}
		
	}
	//glDeleteTextures(1, &texture);
}

