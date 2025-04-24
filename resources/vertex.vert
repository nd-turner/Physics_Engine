#version 330 core

layout (location = 0) in vec3 p;

uniform mat4 transform;



void main()
{
	
	gl_Position = transform * vec4(p, 1);

}