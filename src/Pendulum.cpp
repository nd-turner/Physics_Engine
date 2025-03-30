
#pragma once

#include "Pendulum.h"
#include <vector>



Pendulum::Pendulum(float InitPos[3], float InitVel[3], float length, float angle)
    : Object(InitPos, InitVel), length(length), angle(angle),
    angularVelocity(0.0f), angularAcceleration(0.0f)
{
  
}


Pendulum::~Pendulum()
{
   
}


std::vector<Vertex> Pendulum::generateMesh() {
    std::vector<Vertex> GeoMesh;

    const float centerX = Pos[0];
    const float centerY = Pos[1];
    const float centerZ = Pos[2];

    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;

    // Generate the rectangular shaft vertices
    for (int i = 0; i < resolution; ++i) {
        for (int j = 0; j < resolution; ++j) {
            float x = centerX + (i / float(resolution - 1)) * width - halfWidth;
            float y = centerY + (j / float(resolution - 1)) * height - halfHeight;
            float z = centerZ;

            GeoMesh.emplace_back(Vertex{ x, y, z });
        }
    }

    // Circular base generation
    int baseStartIndex = GeoMesh.size(); // Base vertices start after the shaft

    // Add the center of the circular base
    GeoMesh.emplace_back(Vertex{ centerX, centerY - height / 2.0f, centerZ });

    for (int i = 0; i < resolution; ++i) {
        float theta = 2.0f * 3.14159f * static_cast<float>(i) / resolution;
        float x = centerX + Rad * cos(theta);
        float y = centerY - height / 2.0f + Rad * sin(theta);
        float z = centerZ;

        GeoMesh.emplace_back(Vertex{ x, y, z });
    }

    return GeoMesh;
}

std::vector<int> Pendulum::generateElem(const std::vector<Vertex> GeoMesh) {
    std::vector<int> GeoElem;
    int baseStartIndex = resolution * resolution;
    int baseResolution = resolution;

    // Generate indices for the rectangular shaft
    for (int i = 0; i < resolution - 1; ++i) {
        for (int j = 0; j < resolution - 1; ++j) {
            int topLeft = i * resolution + j;
            int topRight = (i + 1) * resolution + j;
            int bottomLeft = i * resolution + (j + 1);
            int bottomRight = (i + 1) * resolution + (j + 1);

            GeoElem.push_back(topLeft);
            GeoElem.push_back(bottomLeft);
            GeoElem.push_back(topRight);

            GeoElem.push_back(topRight);
            GeoElem.push_back(bottomLeft);
            GeoElem.push_back(bottomRight);
        }
    }

    // Generate indices for the circular base
    int centerIndex = baseStartIndex;

    for (int i = 0; i < baseResolution; ++i) {
        int next = (i + 1) % baseResolution; // Wrap around to close the circle
        GeoElem.push_back(centerIndex);
        GeoElem.push_back(baseStartIndex + i + 1);
        GeoElem.push_back(baseStartIndex + next + 1);
    }

    return GeoElem;
}
