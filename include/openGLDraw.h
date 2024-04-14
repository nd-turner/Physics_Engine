#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <vector>
#include "Vertex.h"


//lets create a class called draw that will handle the back end of all of the rendering

class draw {

public:
	draw(std::vector <Vertex> mesh, std::vector<int> elem) {

	}

	static void uploadMesh(std::vector <Vertex>& mesh, std::vector<int> elem) {

		uint32_t vao, vbo, ebo;





	}

};