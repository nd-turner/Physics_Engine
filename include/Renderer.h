
#pragma once

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <vector>
#include "Vertex.h"

struct MeshData {
	uint32_t VAO;
	uint32_t VBO;
	uint32_t EBO;
	//uint32_t numIndices;
};

class Renderer {

public:
	Renderer();
	~Renderer();
	MeshData uploadMesh(std::vector <Vertex>& mesh, std::vector<int> elem);
	void unloadMesh(MeshData& mesh);
	void drawMesh(MeshData& mesh, uint32_t numIndices);

};