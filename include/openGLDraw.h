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

	static uint32_t uploadMesh(std::vector <Vertex>& mesh, std::vector<int> elem) {

		uint32_t VAO, VBO, EBO;

		//generate and bind VAO
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(Vertex), mesh.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Pos));
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elem.size() * sizeof(uint32_t), elem.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);


		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);

		return VAO;

		

	}
	void unloadMesh(uint32_t VAO) {

		glDeleteVertexArrays(1, &VAO);
	}

	void drawMesh(uint32_t VAO, uint32_t numIndices) {
		// Bind the provided VAO
		glBindVertexArray(VAO);

		// Draw the mesh using the number of indices
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);

		// Unbind the VAO
		glBindVertexArray(0);
	}
};