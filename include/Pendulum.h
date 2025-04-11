#pragma once

#include "Object.h"
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Pendulum : public Object {

	private:
		float length;
		float angle;
		float angularVelocity;  
		float angularAcceleration;
		float massRad = 0.15f;
		int resolution = 100;
		float width = 0.05f;
		float height = 1.0f;

		float pivotX;
		float pivotY;
		glm::mat4 modelMatrix;

	public:

		Pendulum(float InitPos[3], float InitVel[3], float length, float angle, float massRad);
		~Pendulum();

		//rendering information
		std::vector<Vertex> generateMesh() override;
		std::vector<int> generateElem(std::vector<Vertex> GeoMesh) override;

		void pivot(float angle);
		glm::mat4 getModelMatrix();

};