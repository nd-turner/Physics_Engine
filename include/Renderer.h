
#pragma once

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <vector>
#include "Vertex.h"


class Renderer {

public:
	Renderer();
	~Renderer();
	uint32_t uploadMesh(std::vector <Vertex>& mesh, std::vector<int> elem);
	void unloadMesh(uint32_t VAO);
	void drawMesh(uint32_t VAO, uint32_t numIndices);

};