
#ifndef TIMER_H
#define TIMER_H

#include <GLFW/glfw3.h>


struct timer {

	double startTime = 0;
	int ticks = 0;
	const int tickRate = 10;


	//basically this function just adds some delay on the actual tick, but once that delay has passed there is no more delay.
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
#endif // TIMER_H
