#pragma once

#include "Object.h"
#include <cmath>

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

	public:

		Pendulum(float InitPos[3], float InitVel[3], float length, float angle, float massRad);
		~Pendulum();

		//rendering information
		std::vector<Vertex> generateMesh() override;
		std::vector<int> generateElem(std::vector<Vertex> GeoMesh) override;


};