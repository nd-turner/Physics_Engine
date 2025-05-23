
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stb_image/stb_image.h>
#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
bool checkCollisionDetection( Object* circle1, Object* circle2) {

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
void swing(Pendulum& Pendulum1) {
	static float time = 0.0f;       // keeps track of time between frames
	float amplitude = 90;        // max angle in degrees
	float frequency = 1.0f;         // swing frequency in Hz
	float damper = .5;

	time += tickMaster.getDt() * .25; // approximate frame time (60 FPS); replace with actual dt if available

	
	float angle = amplitude * damper * std::sin(2 * 3.14159f * frequency * time) ;
	Pendulum1.setAngle(angle);      // angle in degrees, assuming setAngle converts to radians
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
	GLuint sID = s.getID();

	glm::vec4 color = glm::vec4(1., 1., 1., 1);
	glm::mat4 trans = glm::mat4(1.0);

	float angle = 180.0f;
	
	trans = glm::rotate(trans, angle, glm::vec3(0.0f, 0.0f, 0.0f));
	s.getUniform(sID, "ourColor");
	s.setUniform("ourColor", color);

	s.getUniform(sID, "transform");
	s.setUniformMatrix("transform", trans);


	int startTick = tickMaster.getTicks();
	int loopCount = 0;

	

	float InitPos[3] = { 0.0f, 1.3f, 0.0f };
	float InitVel[3] = { 0.0f, 0.0f, 0.0f };

	float length = 1.0f;
	float Circ1Rad = 0.1f;
	

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

	Pendulum* Pendulum1 = new Pendulum(InitPos, InitVel, length, 0, 0.1f);
	Pendulum1->setRenderer(&Renderer);
	GameObjects.push_back(Pendulum1);
	
	Pendulum1->setAngle(angle);

	Box* top = new Box(TopInitPos, TopInitVel, 0.05f, 1.0f);
	top->setRenderer(&Renderer);
	GameObjects.push_back(top);

	glfwSetWindowUserPointer(window, Pendulum1);

	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);

	/*Pendulum1->pivot(angle);

	glm::mat4 model = Pendulum1->getModelMatrix();

	model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));*/

	//GLuint modelLoc = glGetUniformLocation(vertex.vert, "model");
	//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


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


		if (currentTick - startTick >= 1) {
			startTick = currentTick;
			loopCount++;

			for (int i = 0; i < GameObjects.size(); i++) {
				bool isAnyDraggable = false;

				const float* pos = GameObjects[i]->getPosition();
				const float* vel = GameObjects[i]->getVelocity();

				float newPosition[3];

				for (int j = 0; j < 3; j++) {
					newPosition[j] = pos[j] + vel[j] * dt;
				}

				GameObjects[i]->updatePosition(newPosition);
				swing(*Pendulum1);
				Pendulum1->update();
				
				

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
						float newPos[3] = { normMx, normMy, 0.0f };

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

		for (auto& object : GameObjects) {
			object->draw(s);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

