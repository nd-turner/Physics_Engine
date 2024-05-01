
#pragma once

#include <GLFW/glfw3.h>


class Timer {
	private:
		double startTime = 0;
		int ticks = 0;
		const int tickRate = .5;	//rate of ticking lower the number faster the tick

	public:
		Timer();
		~Timer();
		void tick();
		int getTicks();
};

