#pragma once

#include "Timer.h"

Timer::Timer() {}
Timer::~Timer() {}

void Timer::tick() {
	
	double currentTime = glfwGetTime();
	dt = currentTime - lastTime;  // Calculate delta time
	lastTime = currentTime;

	if (currentTime - startTime >= tickRate) { // Check if one second has passed
		ticks++;
		startTime = currentTime;
	}
}

double Timer::getDt() {
	return dt;
}

int Timer::getTicks() {

	return ticks;
}