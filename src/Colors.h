#ifndef COLORS_H
#define COLORS_H
#pragma once

#include <SDL.h>

namespace Colors {
	const SDL_Color BLACK = { 000, 000, 000, 255 };
	const SDL_Color DARK_GREY = { 064, 064, 064, 255 };
	const SDL_Color GREY = { 128, 128, 128, 255 };
	const SDL_Color LIGHT_GREY = { 192, 192, 192, 255 };
	const SDL_Color WHITE = { 255, 255, 255, 255 };

	const SDL_Color RED = { 255, 000, 000, 255 };
	const SDL_Color ORANGE = { 255, 128, 000, 255 };
	const SDL_Color YELLOW = { 255, 255, 000, 255 };
	const SDL_Color LIME = { 128, 255, 000, 255 };
	const SDL_Color GREEN = { 000, 255, 000, 255 };
	const SDL_Color SEAFOAM = { 000, 255, 128, 255 };
	const SDL_Color CYAN = { 000, 255, 255, 255 };
	const SDL_Color SAPPHIRE = { 000, 128, 255, 255 };
	const SDL_Color BLUE = { 000, 000, 255, 255 };
	const SDL_Color PURPLE = { 128, 000, 255, 255 };
	const SDL_Color MAGENTA = { 255, 000, 255, 255 };
	const SDL_Color PINK = { 255, 000, 128, 255 };
	// http://dawnsbrain.com/wp-content/uploads/2009/03/rgb-color-wheel-lg.jpg
}

#endif // !COLORS_H