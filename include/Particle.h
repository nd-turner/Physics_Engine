#pragma once

#include "Object.h"
#include <cmath>

class Particle : public Object {

private:

	float Rad = 0.15f;
	int resolution = 100;
	

public:

	Particle(float InitPos[3], float InitVel[3], float Rad);
	~Particle();

	//rendering information
	std::vector<Vertex> generateMesh() override;
	std::vector<int> generateElem(std::vector<Vertex> GeoMesh) override;

	void handleWallCollision();
	float getRad();


};