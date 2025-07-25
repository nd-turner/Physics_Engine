
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
#include "Constants.h"
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

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

}

void swing(Pendulum& Pendulum1) {
	static float time = 0.0f;
	float amplitude = 90;
	float frequency = 1.0f;
	float damper = .5;

	time += tickMaster.getDt() * .25;

	float angle = amplitude * damper * std::sin(2 * 3.14159f * frequency * time) ;
	Pendulum1.setAngle(angle);
}


int main(void)
{
	glfwSetErrorCallback(glfw_Error_Callback);
	if (!glfwInit()) { return -1; }

#pragma region report opengl errors to std
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

	Renderer Renderer;
	Force forces;

	std::vector<Object*> GameObjects;

	Pendulum* Pendulum1 = new Pendulum(Pendulum_1::InitPos, Pendulum_1::InitVel, Pendulum_1::length, Pendulum_1::PendulumAngle, Pendulum_1::massRad);
	Pendulum1->setRenderer(&Renderer);
	GameObjects.push_back(Pendulum1);
	
	Pendulum1->setAngle(angle);

	Box* top = new Box(bar::TopInitPos, bar::TopInitVel, bar::boxHeight, bar::boxWidth);
	top->setRenderer(&Renderer);
	GameObjects.push_back(top);

	glfwSetWindowUserPointer(window, Pendulum1);

	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);


	glm::mat4 model = Pendulum1->getModelMatrix();

	model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

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
						GameObjects[i]->updateVelocity(0);
					}
				}
				else {
					glfwSetCursor(window, glfwCreateStandardCursor(GLFW_ARROW_CURSOR));
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

