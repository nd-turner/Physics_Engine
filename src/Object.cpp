
#pragma once


#include "Object.h"

Object::Object(float InitPos[3], float InitVel[3]) {
    
    for (int i = 0; i < 3; ++i) {
        this->Pos[i] = InitPos[i];
        this->Vel[i] = InitVel[i];
    }
    

    this->vertices = generateMesh();
    this->elements = generateElem(vertices);
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

void Object::setRenderer(Renderer* r) {

    this->renderer = r;
}

void Object::draw(Shader& shader) {


    if (meshData.VAO == 0) {
        meshData = renderer->uploadMesh(vertices, elements);
    }

    shader.bind();
    shader.setUniformMatrix("transform", modelMatrix);

    renderer->drawMesh(meshData, elements.size());

}

