#pragma once

#include "Vertex.h" 
#include <vector>
#include "Object.h" 

/*
	here we will start to apply forces to objects obeying the laws of newtionian mechanics

	1st law: Inerta

	2nd law: Fnet = MA

	3rd law: Action-Reaction

*/



class Force {
	private:

		float magnitude;
		float direction[3];

	public:

		void genForce(Object& obj, float magnitude, float direction[3]);	//Apply generic force to an object
		void gravity(Object* obj);	//Apply gravity to an object
		void normal(Object& obj);	//Apply normal force to an object


		

		


};