#ifndef WINDOW_H
#define WINDOW_H
#pragma once

#include <SDL_image.h>
#include <string>
#include "Vector2i.h"

namespace Window {
	inline SDL_Window* window = nullptr;
	inline SDL_Renderer* renderer = nullptr;

	void Init(std::string title = "Game", Vector2i dim = { 128, 96 });
	void HandleEvents();
	void PresentWindow();

	void SetLetterboxColor(SDL_Color color);
	void SetWindowIcon(std::string filePath);
	std::string GetWindowTitle();
	void SetWindowTitle(std::string title);
	Vector2i GetWindowSize();
	void SetWindowSize(Vector2i dim);
	SDL_Texture* GetRenderTarget();
	void SetRenderTarget(SDL_Texture* texture);
	SDL_Rect GetViewportRect();
	Vector2i GetViewportPos();
	Vector2i GetViewportSize();
}

#endif // !WINDOW_H