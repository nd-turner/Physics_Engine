
#pragma once

#include "Vertex.h" 
#include <vector>

#include "Object.h"

Object::Object(float InitPos[3], float InitVel[3]) {
    
    for (int i = 0; i < 3; ++i) {
        this->Pos[i] = InitPos[i];  // Copy InitPos elements
        this->Vel[i] = InitVel[i];  // Copy InitVel elements
    }

};

void Object::updatePosition(float pos[3]) {
    for (int i = 0; i < 3; i++) {
        this->Pos[i] = pos[i];
    }
};

void Object::updateVelocity(float vel[3]) {
    for (int i = 0; i < 3; i++) {
        this->Vel[i] = vel[i];
    }

};

//is object colliding with another wall
bool Object::isColliding() const {

    if (abs(Pos[1]) > 1 || abs(Pos[0]) > 1) {

        return true;

    }


    return false;
};

void Object::handleWallCollision() {

    // Boundary values
    const float error = 0.0f;
    const float boundary = 1.0f-error;
    const float damping = 0.75f;

    // Handle collision with the walls
    if (Pos[0] > boundary-Rad) {
        Pos[0] = boundary-Rad;          // Correct position
        Vel[0] *= -damping;        // Reflect and dampen velocity
    }
    else if (Pos[0] < -boundary+Rad) {
        Pos[0] = -boundary+Rad;         // Correct position
        Vel[0] *= -damping;        // Reflect and dampen velocity
    }

    else if (Pos[1] > boundary-Rad) {
        Pos[1] = boundary-Rad;          // Correct position
        Vel[1] *= -damping;        // Reflect and dampen velocity
    }
    else if (Pos[1] < -boundary+Rad) {
        Pos[1] = -boundary+Rad;         // Correct position
        Vel[1] *= -damping;        // Reflect and dampen velocity
    }
};

const float* Object::getPosition() const {
    return Pos;
};

const float* Object::getVelocity() const {
    return Vel;
};

const float Object::getRad() const {
    return Rad;
};

const float Object::getMass() const {

    return Mass;
};

std::vector<Vertex> Object::generateMesh() {
    std::vector<Vertex> GeoMesh;

    const float centerX = Pos[0];
    const float centerY = Pos[1];
    int Res = resolution;

    for (int i = 0; i < Res; ++i) {
        float theta = 2 * 6.0f * static_cast<float>(i) / Res;
        float x = centerX + Rad * cos(theta);
        float y = centerY + Rad * sin(theta);
        float z = Pos[2];

        GeoMesh.emplace_back(x, y, z);
    }

    return GeoMesh;
}

std::vector<int> Object::generateElem(std::vector<Vertex> GeoMesh) {
    std::vector<int> GeoElem;

    int centerIndex = 0;

    for (int i = 1; i < GeoMesh.size() - 1; ++i) {
        GeoElem.push_back(centerIndex);
        GeoElem.push_back(i);
        GeoElem.push_back(i + 1);
    }


    GeoElem.push_back(centerIndex);
    GeoElem.push_back(GeoMesh.size() - 1);
    GeoElem.push_back(1);



    return GeoElem;
}

std::vector<Vertex> Object::generateCircleMesh() {
	std::vector<Vertex> GeoMesh;

    const float centerX = Pos[0];
    const float centerY = Pos[1];
    int Res = resolution;

        for (int i = 0; i < Res; ++i) {
            float theta = 2 * 3.14159f * static_cast<float>(i) / Res; 
            float x = centerX + Rad * cos(theta);
            float y = centerY + Rad * sin(theta);
            float z = Pos[2];

            GeoMesh.emplace_back(x, y, z);
        }
	
	return GeoMesh;
}

std::vector<int> Object::generateCircleElem(std::vector<Vertex> GeoMesh) {
	std::vector<int> GeoElem;

        int centerIndex = 0;

        for (int i = 1; i < GeoMesh.size() - 1; ++i) {
            GeoElem.push_back(centerIndex);
            GeoElem.push_back(i);
            GeoElem.push_back(i + 1);
        }

        
        GeoElem.push_back(centerIndex);
        GeoElem.push_back(GeoMesh.size() - 1);
        GeoElem.push_back(1);
    

   
	return GeoElem;
}

std::vector<Vertex> Object::generateRectangleMesh() {
    std::vector<Vertex> GeoMesh;

    const float centerX = Pos[0];
    const float centerY = Pos[1];
    int Res = resolution;

   
   

    // Half-width and half-height for positioning vertices relative to the center
    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;

    // Loop to generate the rectangle mesh
    for (int i = 0; i < Res; ++i) {
        for (int j = 0; j < Res; ++j) {
            // Calculate the normalized position of the current vertex in the grid
            float x = centerX + (i / float(Res - 1)) * width - halfWidth;
            float y = centerY + (j / float(Res - 1)) * height - halfHeight;
            float z = Pos[2];

            GeoMesh.emplace_back(x, y, z);
        }
    }


    return GeoMesh;
}

std::vector<int> Object::generateRectangleElem(std::vector<Vertex> GeoMesh) {
    std::vector<int> elements;

    int Res = resolution; // Resolution of the grid
    int numVertices = GeoMesh.size(); // Total number of vertices

    // Loop through the grid and generate two triangles for each 2x2 grid of vertices
    for (int i = 0; i < Res - 1; ++i) {
        for (int j = 0; j < Res - 1; ++j) {
            // Indices of the current 2x2 block of vertices
            int topLeft = i * Res + j;            // Top-left corner
            int topRight = i * Res + (j + 1);    // Top-right corner
            int bottomLeft = (i + 1) * Res + j;  // Bottom-left corner
            int bottomRight = (i + 1) * Res + (j + 1);  // Bottom-right corner

            // First triangle: (top-left, top-right, bottom-left)
            elements.push_back(topLeft);
            elements.push_back(bottomLeft);
            elements.push_back(topRight);

            // Second triangle: (top-right, bottom-left, bottom-right)
            elements.push_back(topRight);
            elements.push_back(bottomLeft);
            elements.push_back(bottomRight);
        }
    }

    return elements;
}
