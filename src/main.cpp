
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
#include <chrono>

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

//implement simple collision detection based of the circlular nature of the object (will implement SAT for better performance down the road)
bool checkCollisionDetection(const Object& circle1, const Object& circle2) {

	//lets get the position of each circle
	const float* pos1 = circle1.getPosition();
	const float* pos2 = circle2.getPosition();

	//lets get the radius of each circle
	float rad1 = circle1.getRad();
	float rad2 = circle2.getRad();

	//lets check if the sum of the radius's ^2 is smaller than the distance between the circles
	float radiSumSquared = (rad1 + rad2) * (rad1 + rad2);

	float dx = pos2[0] - pos1[0];
	float dy = pos2[1] - pos1[1];

	dx *= dx;
	dy *= dy;

	float distanceSquared = dx + dy;


	return distanceSquared <= radiSumSquared;
}

void HandleCollision( Object& circle1,  Object& circle2) {

	//lets get the position of each circle
	const float* pos1 = circle1.getPosition();
	const float* pos2 = circle2.getPosition();

	//lets get the radius of each circle
	float rad1 = circle1.getRad();
	float rad2 = circle2.getRad();

	//lets check if the sum of the radius's ^2 is smaller than the distance between the circles
	float radiSumSquared = (rad1 + rad2) * (rad1 + rad2);

	float dx = pos2[0] - pos1[0];
	float dy = pos2[1] - pos1[1];


	// Calculate the distance between the circles
	float distance = std::sqrt(dx * dx + dy * dy);
	float overlap = (rad1 + rad2) - distance;
	


	// If there is an overlap, resolve it
	if (overlap > 0) {
		// Normalize the collision vector
		float nx = dx / distance;
		float ny = dy / distance;

		// Move circles apart based on overlap
		float halfOverlap = overlap / 2.0f;
		float newPos1[3] = { pos1[0] - nx * halfOverlap, pos1[1] - ny * halfOverlap, 0.0f };
		float newPos2[3] = { pos2[0] + nx * halfOverlap, pos2[1] + ny * halfOverlap, 0.0f };

		circle1.updatePosition(newPos1);
		circle2.updatePosition(newPos2);

		// Calculate new velocities after collision
		const float* vel1 = circle1.getVelocity();
		const float* vel2 = circle2.getVelocity();

		// Simple elastic collision response (reflecting velocities)
		float newVel1[3] = { -vel1[0], -vel1[1], 0.0f }; // Reflect velocity for circle1
		float newVel2[3] = { -vel2[0], -vel2[1], 0.0f }; // Reflect velocity for circle2

		circle1.updateVelocity(newVel1);
		circle2.updateVelocity(newVel2);
	}


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

	int numCirc = 5;
	
	float InitPos[3] = { 0.0f, 0.7f, 0.0f };
	float InitVel[3] = { -1.0f, 3.0f, 0.0f };
	float Circ1Rad = 0.1f;

	float InitPos2[3] = { 0.5f, -0.5f, 0.0f };
	float InitVel2[3] = { 1.0f, 3.0f, 0.0f };
	float Circ1Rad2 = 0.1f;

	float InitPos3[3] = { -0.6f, 0.4f, 0.0f };
	float InitVel3[3] = { 0.8f, -2.0f, 0.0f };
	float Circ1Rad3 = 0.1f;

	float InitPos4[3] = { -0.3f, -0.9f, 0.0f };
	float InitVel4[3] = { 0.5f, 1.5f, 0.0f };
	float Circ1Rad4 = 0.1f;

	float InitPos5[3] = { -0.3f, 0.9f, 0.0f };
	float InitVel5[3] = { -0.5f, -1.5f, 0.0f };
	float Circ1Rad5 = 0.1f;

	float InitPos6[3] = { 0.3f, -0.9f, 0.0f };
	float InitVel6[3] = { -0.5f, 1.0f, 0.0f };
	float Circ1Rad6 = 0.1f;

	std::vector<Object> GameObjects;
	
	
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

	
	//
	
	Object circle1(InitPos, InitVel);
	circle1.setRad(.1);

	Object circle2(InitPos2, InitVel2);
	circle2.setRad(.1);

	Object circle3(InitPos3, InitVel3);
	circle3.setRad(.1);

	Object circle4(InitPos4, InitVel4);
	circle4.setRad(.1);

	Object circle5(InitPos5, InitVel5);
	circle5.setRad(.1);

	Object circle6(InitPos6, InitVel6);
	circle6.setRad(.1);

	GameObjects.push_back(circle1);
	GameObjects.push_back(circle2);
	GameObjects.push_back(circle3);
	GameObjects.push_back(circle4);
	GameObjects.push_back(circle5);
	GameObjects.push_back(circle6);


	while (!glfwWindowShouldClose(window))
	{
		
		int width = 0, height = 0;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		tickMaster.tick();
		int currentTick = tickMaster.getTicks();
		float dt = static_cast<float>(tickMaster.getDt());

		//mesh and elem matrix for the entire screen
		std::vector<Vertex> mesh;
		std::vector<int> elem;

		
		int vertexOffset = 0;  // Track the current number of vertices

		

		for (int i = 0; i < GameObjects.size(); i++) {

			std::vector<Vertex> TempMesh = GameObjects[i].generateCircleMesh();
			std::vector<int> TempElem = GameObjects[i].generateCircleElem(TempMesh);

			// Concatenate vertices and elements from the current game object
			mesh.insert(mesh.end(), TempMesh.begin(), TempMesh.end());

			for (int& elemIndex : TempElem) {
				elemIndex += vertexOffset;  // Offset the indices
			}


			elem.insert(elem.end(), TempElem.begin(), TempElem.end());

			vertexOffset += TempMesh.size();
		}

			std::cout << "Mesh size: " << mesh.size() << " Elem size: " << elem.size() << std::endl;

		if (currentTick - startTick >= 1) {		//game loop
			startTick = currentTick;
			loopCount++;

			for (int i = 0; i < GameObjects.size(); i++) {
				
			}

			//go through all game objects
			for (int i = 0; i < GameObjects.size(); i++) {

				const float* pos = GameObjects[i].getPosition();
				const float* vel = GameObjects[i].getVelocity();

				float newPosition[3];

				for (int j = 0; j < 3; j++) {
					newPosition[j] = pos[j] + vel[j] * dt;
				}

				GameObjects[i].updatePosition(newPosition);
				if (GameObjects[i].isColliding()) {
					GameObjects[i].handleWallCollision();

				}

				for (int j = i + 1; j < GameObjects.size(); j++) {

					if (checkCollisionDetection(GameObjects[i], GameObjects[j])) {
						
						//a collision has occured lets handle it
						HandleCollision(GameObjects[i], GameObjects[j]);

					}
				}
				
			}
				

		}

		uint32_t vao = Renderer.uploadMesh(mesh, elem);
		Renderer.drawMesh(vao, elem.size());
		Renderer.unloadMesh(vao);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//glDeleteTextures(1, &texture);
}

