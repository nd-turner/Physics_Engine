
#pragma once

#include <GLFW/glfw3.h>


class Timer {
	private:
		double startTime = 0;

		int ticks = 0;
		const float tickRate = 1/60;

		double dt = 0.0;
		double lastTime = 0;

	public:
		Timer();
		~Timer();
		void tick();
		int getTicks();
		double getDt();
};

