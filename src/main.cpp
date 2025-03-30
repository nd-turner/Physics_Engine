
//This is a project where I will be experimenting with opengl in order
//to progress in an exploration of developing a physics engine


#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stb_image/stb_image.h>
#include <random>

#include <cmath>
#include <iostream>
#include <vector>
#include <chrono>



#include "Shader.h"
#include "Callbacks.h"
#include "Level.h"
#include "Timer.h"
#include "Vertex.h"
#include "Renderer.h"
#include "Object.h"
#include "Force.h"
#include "Pendulum.h"
#include "Box.h"
#include "Particle.h"


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
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

}



//implement simple collision detection based of the circlular nature of the object (will implement SAT for better performance down the road)
bool checkCollisionDetection(const Object* circle1, const Object* circle2) {

	//lets get the position of each circle
	const float* pos1 = circle1->getPosition();
	const float* pos2 = circle2->getPosition();

	float mass1 = circle1->getMass();
	float mass2 = circle2->getMass();


	//lets get the radius of each circle
	float rad1 = circle1->getRad();
	float rad2 = circle2->getRad();

	//lets check if the sum of the radius's ^2 is smaller than the distance between the circles
	float radiSumSquared = (rad1 + rad2) * (rad1 + rad2);

	float dx = pos2[0] - pos1[0];
	float dy = pos2[1] - pos1[1];

	dx *= dx;
	dy *= dy;

	float distanceSquared = dx + dy;


	return distanceSquared <= radiSumSquared;
}

void HandleCollision(Object* circle1, Object* circle2) {

	//lets get the position of each circle
	const float* pos1 = circle1->getPosition();
	const float* pos2 = circle2->getPosition();

	//lets get the radius of each circle
	float rad1 = circle1->getRad();
	float rad2 = circle2->getRad();

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

		circle1->updatePosition(newPos1);
		circle2->updatePosition(newPos2);

		// Calculate new velocities after collision
		const float* vel1 = circle1->getVelocity();
		const float* vel2 = circle2->getVelocity();

		// Simple elastic collision response (reflecting velocities)
		float newVel1[3] = { -vel1[0], -vel1[1], 0.0f }; // Reflect velocity for circle1
		float newVel2[3] = { -vel2[0], -vel2[1], 0.0f }; // Reflect velocity for circle2

		circle1->updateVelocity(newVel1);
		circle2->updateVelocity(newVel2);
	}


}

int main(void)
{
	glfwSetErrorCallback(glfw_Error_Callback);
	if (!glfwInit()) { return -1; }

#pragma region report opengl errors to std
	//this code enables GLFW debugging
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#pragma endregion


	GLFWwindow* window = window = glfwCreateWindow(640, 640, "Sim_Window", NULL, NULL);
	if (!window) { glfwTerminate(); return -1; }

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

	float InitPos[3] = { 0.0f, 0.3f, 0.0f };
	float InitVel[3] = { 0.0f, 0.0f, 0.0f };

	//pendulum 1
	float length = 1.0f;
	float angle = 0.5f;
	float Circ1Rad = 0.1f;

	float circle2IinitPos[3] = { 0.2f,0.7f,0.0f };
	float circle3IinitPos[3] = { 0.4f,0.7f,0.0f };
	float circle4IinitPos[3] = { 0.6f,0.7f,0.0f };
	float circle5IinitPos[3] = { 0.8f,0.7f,0.0f };

	float TopInitPos[3] = { 0.0f, 0.8f, 0.0f };
	float TopInitVel[3] = { 0.0f, 0.0f, 0.0f };

	

	/*texture

	int Textwidth, Textheight, TextChannels;
	unsigned char* image_data = stbi_load("wood.png", &Textwidth, &Textheight, &TextChannels, STBI_rgb);

	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	scaling
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	repeating
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Textwidth, Textheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	glGenerateMipmap(GL_TEXTURE_2D);


	stbi_image_free(image_data);
	glBindTexture(GL_TEXTURE_2D, 0);*/



	Renderer Renderer;
	Force forces;

	std::vector<Object*> GameObjects;

	Pendulum* Pendulum1 = new Pendulum(InitPos, InitVel, length, angle, 0.1f);
	GameObjects.push_back(Pendulum1);

	Box* top = new Box(TopInitPos, TopInitVel, 0.05f, 1.0f);
	GameObjects.push_back(top);

	Particle* test = new Particle(circle2IinitPos, InitVel, 0.5f);
	GameObjects.push_back(test);

	glfwSetWindowUserPointer(window, &Pendulum1);

	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);

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

		std::vector<Vertex> mesh;
		std::vector<int> elem;

		int vertexOffset = 0;



		for (int i = 0; i < GameObjects.size(); ++i) {

			std::vector<Vertex> TempMesh = GameObjects[i]->generateMesh();
			std::vector<int> TempElem = GameObjects[i]->generateElem(TempMesh);

			mesh.insert(mesh.end(), TempMesh.begin(), TempMesh.end());

			for (int& elemIndex : TempElem) {
				elemIndex += vertexOffset;  // Offset the indices to avoid conflicts
			}

			elem.insert(elem.end(), TempElem.begin(), TempElem.end());

			vertexOffset += TempMesh.size(); 
		}

		if (currentTick - startTick >= 1) {	//game loop
			startTick = currentTick;
			loopCount++;

			//go through all game objects
			for (int i = 0; i < GameObjects.size(); i++) {
				bool isAnyDraggable = false;

				const float* pos = GameObjects[i]->getPosition();
				const float* vel = GameObjects[i]->getVelocity();

				float newPosition[3];

				for (int j = 0; j < 3; j++) {
					newPosition[j] = pos[j] + vel[j] * dt;
				}

				GameObjects[i]->updatePosition(newPosition);

				//forces.gravity(GameObjects[i]);

				if (GameObjects[i]->isColliding()) {
					GameObjects[i]->handleWallCollision();

				}

				bool drag = isDraggable(window, GameObjects[i]);

				if (drag) {

					glfwSetCursor(window, glfwCreateStandardCursor(GLFW_HAND_CURSOR));
					

					int leftMousePressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

					if (leftMousePressed == GLFW_PRESS) {

						double mouseX, mouseY;
						glfwGetCursorPos(window, &mouseX, &mouseY);


						float normMx = (2.0f * (float)mouseX) / (float)width - 1.0f;
						float normMy = (2.0f * ((float)height - (float)mouseY)) / (float)height - 1.0f;

						float newPos[3] = { normMx, normMy, 0.0f };  // Set z-coordinate as needed


						GameObjects[i]->updatePosition(newPos);
						GameObjects[i]->updateVelocity(InitVel);
					}

				}
				else {
					glfwSetCursor(window, glfwCreateStandardCursor(GLFW_ARROW_CURSOR));
				}

				for (int j = i + 1; j < GameObjects.size(); j++) {
					if (checkCollisionDetection(GameObjects[i], GameObjects[j])) {
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

