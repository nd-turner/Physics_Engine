#version 330 core

layout (location = 0) in vec3 p;


uniform mat4 model;

void main()
{
	
	gl_Position = model * vec4(p, 1);

}