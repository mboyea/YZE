#ifndef DRAW_H
#define DRAW_H
#pragma once

#include <string>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Vector2i.h"
#include "Colors.h"

namespace Window {
	// return the current draw target
	SDL_Texture* GetDrawTarget();
	// change the current draw target
	// a texture equal to NULL returns it to the window
	// return true if it fails
	bool SetDrawTarget(SDL_Texture* texture);
	// set the current draw color
	// return true if it fails
	bool SetDrawColor(const SDL_Color& color);
	// fill the current render target
	// return true if it fails
	bool Paint();
	// fill the current render target with a translucent draw color
	// return true if it fails
	bool PaintAlpha();
	// draw a pixel on the current render target
	// return true if it fails
	bool DrawPixel(const Vector2i& pos);
	// draw a line on the current render target
	// return true if it fails
	bool DrawLine(const Vector2i& a, const Vector2i& b);
	// draw a rectangle on the current render target
	// return true if it fails
	bool DrawRect(const SDL_Rect& rect);
	// draw a filled rectangle on the current render target
	// return true if it fails
	bool DrawFilledRect(const SDL_Rect& rect);
	// draw a circle on the current render target
	// return true if it fails
	bool DrawCircle(const Vector2i& pos, const int& radius);
	// draw a texture on the current render target
	// return true if it fails
	bool DrawTexture(SDL_Texture* texture, const SDL_Rect* rect = NULL, const SDL_Rect* clipRect = NULL);
	// draw a texture on the current render target
	// return true if it fails
	bool DrawTexture(SDL_Texture* texture, const SDL_RendererFlip& flip, const SDL_Rect* rect = NULL, const SDL_Rect* clipRect = NULL, const Vector2i* pivot = NULL, const double& angle = 0);
	// draw text with a transparent background on the current render target
	// return true if it fails
	bool DrawText(std::string text, Vector2i pos, TTF_Font* font, SDL_Color textColor = Colors::WHITE);
	// draw text with an opaque background on the current render target
	// return true if it fails
	bool DrawFilledText(std::string text, Vector2i pos, TTF_Font* font, SDL_Color textColor = Colors::WHITE, SDL_Color backgroundColor = Colors::BLACK);
}

#endif // !DRAW_H