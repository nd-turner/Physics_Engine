#pragma once

#include "Timer.h"

Timer::Timer() {}
Timer::~Timer() {}

void Timer::tick() {
	
	double currentTime = glfwGetTime();
	if (currentTime - startTime >= tickRate) { // Check if one second has passed
		ticks++;
		startTime = currentTime;
	}
}

int Timer::getTicks() {

	return ticks;
}