#pragma once

#include "Vertex.h" 
#include <vector>

//an Object is basically going to be the building block of the physics engine think of it as a physics entity
class Object{

    private:
        std::vector<Vertex> vertices;  // Store vertices for the object
        std::vector<int> elements;

        float Pos[3];   //x y z will get passed to vertices for rendering
        float Vel[3];   //dx and dy
        
        int color[3] = { 48 ,159 ,232 };   //default rgb values

        int resolution = 10000;     // default Resolution for generating mesh gen

    public:
        
        Object(float InitPos[3], float InitVel[3]);
        
        //2d mesh geometry stuff
        std::vector<Vertex> generateCircleMesh(float radius);
        std::vector<int> generateCircleElem(std::vector<Vertex> GeoMesh);

        //Logic
        void updatePosition(float pos[3]);
        void updateVelocity(float vel[3]);

        //collision handling
        void handleWallCollision();
        bool isColliding() const;
        void handleObjectCollision();

        //return functions
        const float* getPosition() const;
        const float* getVelocity() const;


        




};