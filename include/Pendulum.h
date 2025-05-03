#pragma once

#include "Object.h"
#include <cmath>
#include "Renderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Pendulum : public Object {

	private:

		MeshData meshData;
		std::vector<Vertex> vertices;
		std::vector<int> elements;
		glm::mat4 modelMatrix = glm::mat4(1.0);
		Renderer* renderer;
		int resolution = 100;

		float length;
		float angle;
		float angularVelocity;
		float angularAcceleration;
		float massRad = 0.15f;
		float previousAngle = 0.0f;

		float width = 0.05f;
		float height = 1.0f;

		float pivotX;
		float pivotY;
		

	public:

		Pendulum(float InitPos[3], float InitVel[3], float length, float angle, float massRad);
		~Pendulum();

		std::vector<Vertex> generateMesh() override;
		std::vector<int> generateElem(std::vector<Vertex> GeoMesh) override;
		void setRenderer(Renderer* r) override;
		void draw(Shader& shader) override;

		void pivot(float angle);
		void update(float dt);
		glm::mat4 getModelMatrix();

};