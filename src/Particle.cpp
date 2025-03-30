#pragma once

#include "Particle.h"
#include <vector>

Particle::Particle(float InitPos[3], float InitVel[3], float Rad)
    : Object(InitPos, InitVel), Rad(Rad) { 

}

Particle::~Particle()
{

}

std::vector<Vertex> Particle::generateMesh() {
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

std::vector<int> Particle::generateElem(std::vector<Vertex> GeoMesh) {
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