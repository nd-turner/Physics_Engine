#pragma once

#include "Vertex.h" 
#include <vector>

/*what is an object an object is essentially going to be a defined amount of vertex's that will be joined together and liked into some sort of object*/
class Object{
    private:
         
    public:
        
        Object();
        
        //2d geometry stuff
        std::vector<Vertex> generateCircleMesh(float radius, float pos[3], int resolution);
        std::vector<int> generateCircleElem(std::vector<Vertex> GeoMesh);


};