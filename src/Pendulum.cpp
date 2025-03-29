
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

    // Half-width and half-height for positioning vertices relative to the center
    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;

    // Generate the rectangular shaft vertices
    for (int i = 0; i < resolution; ++i) {
        for (int j = 0; j < resolution; ++j) {
            // Calculate the normalized position of the current vertex in the grid
            float x = centerX + (i / float(resolution - 1)) * width - halfWidth;
            float y = centerY + (j / float(resolution - 1)) * height - halfHeight;
            float z = centerZ;

            GeoMesh.emplace_back(Vertex{ x, y, z });
        }
    }

    // Generate the circular base vertices
    int baseResolution = resolution * 2; // Increase resolution for a smoother circle
    int baseStartIndex = resolution * resolution; // Start index for base vertices

    // Add the center of the circular base
    GeoMesh.emplace_back(Vertex{ centerX, centerY - height / 2.0f, centerZ });

    // Generate the perimeter vertices for the circle
    for (int i = 0; i < baseResolution; ++i) {
        float theta = 2.0f * 3.141592653589793f * float(i) / baseResolution;
        float x = centerX + Rad * std::cos(theta);
        float y = centerY - height / 2.0f; // Position the circle at the bottom of the rectangle
        float z = centerZ + Rad * std::sin(theta);

        GeoMesh.emplace_back(Vertex{ x, y, z });
    }

    return GeoMesh;
}

std::vector<int> Pendulum::generateElem(std::vector<Vertex> GeoMesh) {
    std::vector<int> GeoElem;

    int baseStartIndex = resolution * resolution; // Index where the base vertices start
    int baseResolution = resolution * 2; // Base resolution for the circle

    // Generate indices for the rectangular shaft
    for (int i = 0; i < resolution - 1; ++i) {
        for (int j = 0; j < resolution - 1; ++j) {
            int topLeft = i * resolution + j;
            int topRight = (i + 1) * resolution + j;
            int bottomLeft = i * resolution + (j + 1);
            int bottomRight = (i + 1) * resolution + (j + 1);

            // First triangle
            GeoElem.push_back(topLeft);
            GeoElem.push_back(bottomLeft);
            GeoElem.push_back(topRight);

            // Second triangle
            GeoElem.push_back(topRight);
            GeoElem.push_back(bottomLeft);
            GeoElem.push_back(bottomRight);
        }
    }

    // Generate indices for the circular base
    for (int i = 0; i < baseResolution; ++i) {
        int centerIndex = baseStartIndex; // The center of the circular base
        int currentIndex = baseStartIndex + i + 1; // First vertex after the center
        int nextIndex = baseStartIndex + (i + 1) % baseResolution + 1; // Next perimeter vertex

        // Triangle for the base
        GeoElem.push_back(centerIndex);
        GeoElem.push_back(currentIndex);
        GeoElem.push_back(nextIndex);
    }

    return GeoElem;
}