#ifndef INPUT_H
#define INPUT_H
#pragma once

#include <SDL.h>
#include <string>
#include "Vector2i.h"

namespace Input {
	void HandleEvents();
	bool IsKeyDown(SDL_Scancode key);
	bool IsKeyPressed(SDL_Scancode key);
	Vector2i GetMouseWindowPos();
	Vector2i GetMouseViewportPos();
	void StartTextInput(std::string* target, char ignoreChar = '\n');
	void StopTextInput();
	bool DidTextIOUpdate();
}

#endif // !INPUT_H