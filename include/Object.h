#pragma once

#include "Vertex.h" 
#include <vector>
#include "Shader.h"

//an Object is basically going to be the building block of the physics engine think of it as a physics entity
//the idea is for object to basically be the parent class for a variety of shapes

class Object{

    protected:
        std::vector<Vertex> vertices;  // Store vertices for the object
        std::vector<int> elements;

        float Pos[3];   //x y z will get passed to vertices for rendering
        float Vel[3];   //dx and dy
        int color[3] = { 48 ,159 ,232 };   //default rgb values
        int resolution = 100;     // default Resolution for generating mesh gen
        float Mass = 100;

    public:

        bool Drag = false;
        
        Object(float InitPos[3], float InitVel[3]);
        virtual ~Object() {}


        virtual std::vector<Vertex> generateMesh();
        virtual std::vector<int> generateElem(std::vector<Vertex> GeoMesh);
        virtual void draw(Shader& shader);

        void updatePosition(float pos[3]);
        void updateVelocity(float vel[3]);

        bool isColliding() const;
        virtual void handleWallCollision();

        const float* getPosition() const;
        const float* getVelocity() const;
        virtual float getRad();
        const float getMass() const;
        
};