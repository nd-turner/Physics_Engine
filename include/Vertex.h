
#ifndef VERTEX_H
#define VERTEX_H


struct Vertex {

	Vertex(float x, float y, float z) {

		Pos[0] = x;
		Pos[1] = y;
		Pos[2] = z;

	}

	float Pos[3] = {0, 0, 0};

};

#endif