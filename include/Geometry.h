#pragma once

#include "Vertex.h" 
#include <vector>



class Geometry{
    private:
         
    public:

        Geometry();
        std::vector<Vertex> generateCircleMesh(float radius, float pos[3], int resolution);
        std::vector<int> generateCircleElem(std::vector<Vertex> GeoMesh);

        


};