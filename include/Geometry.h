#pragma once

#include "Vertex.h" 
#include <vector>



class Geometry{
    private:
         
    public:
        
        Geometry();

        //2d geometry stuff
        std::vector<Vertex> generateCircleMesh(float radius, float pos[3], int resolution);
        std::vector<int> generateCircleElem(std::vector<Vertex> GeoMesh);

        


};