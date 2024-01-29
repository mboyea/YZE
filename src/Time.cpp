#include "Time.h"
#include "Window.h"
#include <cmath>

static Uint32 timeFrameStart;
static float timeRemainder = 0;

void Time::Wait(Uint32 ms, Uint32 msDelayStep) {
	uint32_t endtime = SDL_GetTicks() + ms;
	for (uint32_t currentTicks = SDL_GetTicks(); currentTicks < endtime; currentTicks = SDL_GetTicks()) {
		if (SDL_GetTicks() < endtime - msDelayStep) {
			SDL_Delay(endtime - currentTicks);
		}
		else {
			SDL_Delay(msDelayStep);
		}

		Window::HandleEvents();
	}
}

void Time::LimitFramerate(unsigned int targetFPS) {
	// calculate constants
	const Uint32 TIME_FRAME_SHOULD_LAST = 1000 / targetFPS;
	const float TIME_FRAME_SHOULD_LAST_REMAINDER = (1000.F / (float)targetFPS) - (float)TIME_FRAME_SHOULD_LAST;

	// increment remainder
	timeRemainder += TIME_FRAME_SHOULD_LAST_REMAINDER;

	// set fame times
	Uint32 timeFrameEnd = SDL_GetTicks();
	deltaTimeMS = timeFrameEnd - timeFrameStart;
	timeFrameStart = timeFrameEnd;

	// calculate time to wait
	int timeToWait = TIME_FRAME_SHOULD_LAST - deltaTimeMS + (Uint32)std::floor(timeRemainder);
	timeRemainder -= std::floor(timeRemainder);

	// wait time
	if (timeToWait >= 0) {
		Wait(timeToWait);

		timeFrameEnd += timeToWait;
		deltaTimeMS += timeToWait + (uint32_t)std::floor(timeRemainder);
		currentFPS = targetFPS;
	}
	else {
		timeRemainder = 0;
		// TODO: check the logic of this, it feels sketchy
		currentFPS = (int)std::ceil(1000.F / deltaTimeMS);
	}

	// update dt
	deltaTime = (float)deltaTimeMS / 1000.f;
}