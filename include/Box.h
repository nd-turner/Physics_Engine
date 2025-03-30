#pragma once

#include "Object.h"
#include <cmath>

class Box : public Object {

private:

	float height;
	float length;

	int resolution = 100;


public:

	Box(float InitPos[3], float InitVel[3], float height, float length);
	~Box();

	//rendering information
	std::vector<Vertex> generateMesh() override;
	std::vector<int> generateElem(std::vector<Vertex> GeoMesh) override;





};