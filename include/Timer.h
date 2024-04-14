
#ifndef TIMER_H
#define TIMER_H

#include <GLFW/glfw3.h>


struct timer {

	double startTime = 0;
	int ticks = 0;
	const int tickRate = .5;	//rate of ticking lower the number faster the tick


	//setup a tick based on a specific rate
	void tick() { 
		double currentTime = glfwGetTime();
		if (currentTime - startTime >= tickRate) { // Check if one second has passed
			ticks++;
			startTime = currentTime;
		}
	}

	int getTicks() {
		return ticks;
	}

	
};
#endif 
