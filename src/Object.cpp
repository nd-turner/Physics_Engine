
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
    const float damping = 0.95f;

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

void Object::handleObjectCollision() {

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

std::vector<Vertex> Object::generateCircleMesh() {
	std::vector<Vertex> GeoMesh;

    const float centerX = Pos[0];
    const float centerY = Pos[1];
    int Res = resolution;

        for (int i = 0; i < Res; ++i) {
            float theta = 2 * 3.14159f * static_cast<float>(i) / Res; //build two halves of the circle
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
