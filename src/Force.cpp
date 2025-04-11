
#pragma once

#include "Force.h"
/*

void genForce(Object& obj, float magnitude, float direction[3]);	//Apply generic force to an object
void gravity(Object& obj);	//Apply gravity to an object
void normal(Object& obj);	//Apply normal force to an object


*/

//apply a generic force to an object
void Force::genForce(Object& obj, float magnitude, float direction[3]) {
    
};

//apply gravity to an object
void Force::gravity(Object* obj) {

	//fnet = ma
    int normalize = 10000;
	float gravConst = -9.81;
	float gravMag = gravConst * obj->getMass();

	float gravAccel[3] = { 0,gravMag/normalize,0 };


    float newVelocity[3];
    const float* currentVel = obj->getVelocity();  // Keep original as const

    // Calculate new velocity
    for (int i = 0; i < 3; i++) {
        newVelocity[i] = currentVel[i] + gravAccel[i];
    }

    obj->updateVelocity(newVelocity);  // Use setVelocity to apply the update


};

//apply normal force to an object
void Force::normal(Object& obj) {



};