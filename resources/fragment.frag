#version 330 core

layout (location = 0) out vec4 color;  // Output color at location 0

uniform vec4 uColor;                   // Uniform color passed from C++

void main()
{
    color = uColor;    // Set the output color to the uniform color
}