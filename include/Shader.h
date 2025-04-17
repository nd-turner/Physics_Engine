#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp> 
#include <glm/gtc/type_ptr.hpp>  
#include <glm/gtc/matrix_transform.hpp>

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

		GLint getUniform(const char *name);
		GLint getUniform(GLuint shaderId, const char* name);

		void setUniform1f(const char* name, float v);
		void setUniform4f(const char* name, float x, float y, float z, float w);
		void setUniform4f(const char* name, const glm::vec4& vec);
		void setUniformMatrix4fv(const char* name, const float* matrix);
		void setUniformMatrix4fv(const char* name, const glm::mat4& matrix);
};


