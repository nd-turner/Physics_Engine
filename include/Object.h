#pragma once

#include "Vertex.h" 
#include <vector>
#include "Shader.h"
#include "Renderer.h"

//an Object is basically going to be the building block of the physics engine think of it as a physics entity
//the idea is for object to basically be the parent class for a variety of shapes

class Object{

    protected:
       
        MeshData meshData;
        std::vector<Vertex> vertices;
        std::vector<int> elements;
        glm::mat4 modelMatrix = glm::mat4(1.0);
        Renderer* renderer;
        int resolution = 100;


        float Pos[3];
        float Vel[3];
        float color[3] = { 48 ,159 ,232};
        float Mass = 100;

    public:

        bool Drag = false;
        
        Object(float InitPos[3], float InitVel[3]);
        virtual ~Object() {}

        virtual std::vector<Vertex> generateMesh();
        virtual std::vector<int> generateElem(std::vector<Vertex> GeoMesh);
        virtual void draw(Shader& shader);
        virtual void setRenderer(Renderer* r);
      

        void updatePosition(float pos[3]);
        void updateVelocity(float vel[3]);

        bool isColliding() const;
        virtual void handleWallCollision();

        const float* getPosition() const;
        const float* getVelocity() const;
        virtual float getRad();
        const float getMass() const;
        const float* getColor() const;
        
};