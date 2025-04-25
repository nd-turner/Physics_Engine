
#pragma once

#include "Pendulum.h"
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Pendulum::Pendulum(float InitPos[3], float InitVel[3], float length, float angle, float massRad)
    : Object(InitPos, InitVel), length(length), angle(angle),massRad(massRad),
    angularVelocity(0.0f), angularAcceleration(0.0f)
{

    pivotX = InitPos[0];
    pivotY = InitPos[1]; 
    angle *= (3.14 / 180.0f);

    Pos[0] = pivotX + length * sin(angle);
    Pos[1] = pivotY - length * cos(angle);

    this->vertices = generateMesh();
    this->elements = generateElem(vertices);

}
    
Pendulum::~Pendulum()
{
   
}

void Pendulum::setRenderer(Renderer* r) {
    
    this->renderer = r;
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
        float x = centerX + massRad * cos(theta);
        float y = centerY - height / 2.0f + massRad * sin(theta);
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

void Pendulum::draw(Shader& shader) {

    
    if (meshData.VAO == 0) {
        meshData = renderer->uploadMesh(vertices, elements);
    }
    
    shader.bind();
    shader.setUniformMatrix("transform", modelMatrix);

    renderer->drawMesh(meshData, elements.size());

}

glm::mat4 Pendulum::getModelMatrix() {
    return modelMatrix;
}

void Pendulum::pivot(float deltaAngle) {
    
}

