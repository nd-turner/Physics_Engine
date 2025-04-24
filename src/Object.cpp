
#pragma once

#include "Vertex.h" 
#include <vector>
#include "Shader.h"
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

}

float Object::getRad() {

    return 0;
}

const float* Object::getPosition() const {
    return Pos;
};

const float* Object::getVelocity() const {
    return Vel;
};

const float Object::getMass() const {

    return Mass;
};
const float* Object::getColor() const {
    return color;
}

std::vector<Vertex> Object::generateMesh() {
    std::vector<Vertex> GeoMesh;

    return GeoMesh;
}

std::vector<int> Object::generateElem(std::vector<Vertex> GeoMesh) {
    std::vector<int> GeoElem;

    return GeoElem;
}

//void Object::draw(Shader& shader) {
//
//    //generate mesh and elem matrix
//
//    std::vector<Vertex> GeoMesh = generateMesh();   // Vertices of the object
//    std::vector<int> GeoElem = generateElem(GeoMesh); // Indices to define object geometry
//
//}



