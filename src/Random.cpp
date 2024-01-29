#include "Random.h"
#include <SDL.h>
#include <random>

int Math::Random(int min, int max) {
	// TOOD: seed by nanosecond instead of ms
	std::mt19937 seed(SDL_GetTicks());
	std::uniform_int_distribution<int> dist(min, max);
	return dist(seed);
}