
#ifndef VERTEX_H
#define VERTEX_H


struct Vertex {

	float x, y, z;	//postition
	float r, g, b;	//rgb value of the vertex


	
	Vertex(float _x, float _y, float _z)
		: x(_x), y(_y), z(_z), r(0.0f), g(0.0f), b(0.0f) {}

	Vertex(float _x, float _y, float _z, float _r, float _g, float _b)
		: x(_x), y(_y), z(_z), r(_r), g(_g), b(_b) {}
};

#endif