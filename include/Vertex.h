
#pragma once

struct Vertex {

	Vertex(float x, float y, float z, float r , float g, float b) {

		Pos[0] = x;
		Pos[1] = y;
		Pos[2] = z;
		Col[0] = r;
		Col[1] = g;
		Col[2] = b;
	}

	Vertex(float x, float y, float z) {

		Pos[0] = x;
		Pos[1] = y;
		Pos[2] = z;

	}
	float Pos[3] = {0, 0, 0};
	float Col[3] = { 255, 255, 255 };
 };

