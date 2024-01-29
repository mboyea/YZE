#ifndef TIME_H
#define TIME_H
#pragma once

#include <SDL.h>

namespace Time {
	inline int currentFPS = 0;
	inline float deltaTime = 0;
	inline Uint32 deltaTimeMS = 0;

	void Wait(Uint32 ms, Uint32 msDelayStep = 10);
	void LimitFramerate(unsigned int targetFPS = 60);
}

#endif // !TIME_H