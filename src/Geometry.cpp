#include "Geometry.h"
#pragma once

#include "Vertex.h" 
#include <vector>

#include "Geometry.h"

Geometry::Geometry() {


};

std::vector<Vertex> Geometry::generateCircleMesh(float radius, float pos[3], int resolution) {
	std::vector<Vertex> GeoMesh;

	
        
        const float centerX = pos[0];
        const float centerY = pos[1];

        

        for (int i = 0; i < resolution; ++i) {
            float theta = 2 * 3.14159f * static_cast<float>(i) / resolution; //build two halves of the circle
            float x = centerX + radius * cos(theta);
            float y = centerY + radius * sin(theta);
            float z = pos[2];

            GeoMesh.emplace_back(x, y, z);
        }
	

	return GeoMesh;
}


std::vector<int> Geometry::generateCircleElem(std::vector<Vertex> GeoMesh) {
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
