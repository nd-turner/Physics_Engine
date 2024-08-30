#pragma once

#include "Vertex.h" 
#include <vector>

/*what is an object an object is essentially going to be a defined amount of vertex's that will be joined together and liked into some sort of object*/
class Object{

    private:
        std::vector<Vertex> vertices;  // Store vertices for the object
        std::vector<int> elements;

        float Pos[3];   //x y z will get passed to vertices for rendering
        float Vel[2];   //dx and dy
        int color[3];   //rgb values that will get passed to color the object

        int resolution;     // Resolution for generating mesh gen
    public:
        
        Object();
        
        //2d mesh geometry stuff
        std::vector<Vertex> generateCircleMesh(float radius, float pos[3], int resolution);
        std::vector<int> generateCircleElem(std::vector<Vertex> GeoMesh);

        //Logic
        void updatePosition();

        //collision handling
        void handleWallCollision();
        bool isColliding(const Object& other) const;
        void handleObjectCollision();

        //return functions
        const float* getPosition() const;
        const float* getVelocity() const;


        




};