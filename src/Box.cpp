
#pragma once

#include "Box.h"
#include <vector>

Box::Box(float InitPos[3], float InitVel[3], float height, float length)
    : Object(InitPos, InitVel), height(height),length(length) {

}

Box::~Box(){

}

std::vector<Vertex> Box::generateMesh() {
    std::vector<Vertex> GeoMesh;

    const float centerX = Pos[0];
    const float centerY = Pos[1];
    int Res = resolution;

    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;

    for (int i = 0; i < Res; ++i) {
        for (int j = 0; j < Res; ++j) {
          
            float x = centerX + (i / float(Res - 1)) * width - halfWidth;
            float y = centerY + (j / float(Res - 1)) * height - halfHeight;
            float z = Pos[2];

            GeoMesh.emplace_back(x, y, z);
        }
    }

    return GeoMesh;
}

std::vector<int> Box::generateElem(std::vector<Vertex> GeoMesh) {
    std::vector<int> elements;

    int Res = resolution;
    int numVertices = GeoMesh.size(); 

    for (int i = 0; i < Res - 1; ++i) {
        for (int j = 0; j < Res - 1; ++j) {
         
            int topLeft = i * Res + j;           
            int topRight = i * Res + (j + 1);    
            int bottomLeft = (i + 1) * Res + j;  
            int bottomRight = (i + 1) * Res + (j + 1); 

            elements.push_back(topLeft);
            elements.push_back(bottomLeft);
            elements.push_back(topRight);

            elements.push_back(topRight);
            elements.push_back(bottomLeft);
            elements.push_back(bottomRight);
        }
    }

    return elements;
}