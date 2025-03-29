#pragma once

#include "Vertex.h" 
#include <vector>

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
        float Rad =0.5f;
        float width = 1.5f;
        float height = 0.1f;
        float Mass = 100;

    public:

        bool Drag = false;
        
        Object(float InitPos[3], float InitVel[3]);
        virtual ~Object() {}

        //2d mesh geometry stuff
        std::vector<Vertex> generateCircleMesh();
        std::vector<int> generateCircleElem(std::vector<Vertex> GeoMesh);
        std::vector<Vertex> generateRectangleMesh();
        std::vector<int> generateRectangleElem(std::vector<Vertex> GeoMesh);

        virtual std::vector<Vertex> generateMesh();
        virtual std::vector<int> generateElem(std::vector<Vertex> GeoMesh);

        //Logic
        void updatePosition(float pos[3]);
        void updateVelocity(float vel[3]);

        void setRad(float rad) {
            Rad = rad;
        }

        //collision handling
        virtual void handleWallCollision();
        virtual bool isColliding() const;

        //return functions
        virtual const float* getPosition() const;
        virtual const float* getVelocity() const;
        const float getRad() const;
        virtual const float getMass() const;

};