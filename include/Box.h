#pragma once

#include "Object.h"
#include <cmath>

class Box : public Object {

private:

	MeshData meshData;
	std::vector<Vertex> vertices;
	std::vector<int> elements;
	glm::mat4 modelMatrix = glm::mat4(1.0);
	Renderer* renderer;
	int resolution = 100;

	float height;
	float width;


public:

	Box(float InitPos[3], float InitVel[3], float height, float width);
	~Box();

	std::vector<Vertex> generateMesh() override;
	std::vector<int> generateElem(std::vector<Vertex> GeoMesh) override;

	void setRenderer(Renderer* r) override;
	void draw(Shader& shader) override;

};