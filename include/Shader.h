#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
	private:
		GLuint id = 0;

	public:
		bool loadShaderProgramFromData(const char *vertexShaderData, const char *fragmentShaderData);
		bool loadShaderProgramFromData(const char *vertexShaderData,
		const char *geometryShaderData, const char *fragmentShaderData);

		bool loadShaderProgramFromFile(const char *vertexShader, const char *fragmentShader);
		bool loadShaderProgramFromFile(const char *vertexShader,
		const char *geometryShader, const char *fragmentShader);

		void bind();
		void clear();

		GLint getID();
		GLint getUniform(const char* name);
		GLint getUniform(GLuint shaderId, const char* name);

		void setUniform(const char* name, const glm::vec4& value);
		void setUniformMatrix(const char* name, const glm::mat4& value);
};


