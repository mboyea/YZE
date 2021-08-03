#include "Draw.h"
#include "Window.h"

bool Window::SetDrawTarget(SDL_Texture* texture) {
	return SDL_SetRenderTarget(renderer, texture) < 0;
}

SDL_Texture* Window::GetDrawTarget() {
	return SDL_GetRenderTarget(renderer);
}

bool Window::SetDrawColor(const SDL_Color& color) {
	return SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) < 0;
}

bool Window::Paint() {
	return SDL_RenderClear(renderer) < 0;
}

bool Window::PaintAlpha() {
	// TODO: paint viewport with transparency via DrawRect
	return false;
}

bool Window::DrawPixel(const Vector2i& pos) {
	return SDL_RenderDrawPoint(renderer, pos.x, pos.y) < 0;
}

bool Window::DrawLine(const Vector2i& a, const Vector2i& b) {
	return SDL_RenderDrawLine(renderer, a.x, a.y, b.x, b.y) < 0;
}

bool Window::DrawRect(const SDL_Rect& rect) {
	return SDL_RenderDrawRect(renderer, &rect) < 0;
}

bool Window::DrawFilledRect(const SDL_Rect& rect) {
	return SDL_RenderFillRect(renderer, &rect) < 0;
}

bool Window::DrawCircle(const Vector2i& pos, const int& radius) {
	const int diameter = radius * 2;
	int x = (radius - 1), y = 0, tx = 1, ty = 1, error = (tx - diameter);

	while (x >= y) {
		//  Each of the following renders an octant of the circle
		if (DrawPixel({ pos.x + x, pos.y - y }) ||
			DrawPixel({ pos.x + x, pos.y + y }) ||
			DrawPixel({ pos.x - x, pos.y - y }) ||
			DrawPixel({ pos.x - x, pos.y + y }) ||
			DrawPixel({ pos.x + y, pos.y - x }) ||
			DrawPixel({ pos.x + y, pos.y + x }) ||
			DrawPixel({ pos.x - y, pos.y - x }) ||
			DrawPixel({ pos.x - y, pos.y + x })) {
			return true;
		}

		if (error <= 0) {
			y++;
			error += ty;
			ty += 2;
		}
		if (error > 0) {
			x--;
			tx += 2;
			error += (tx - diameter);
		}
	}
	return false;
}

bool Window::DrawTexture(SDL_Texture* texture, const SDL_Rect* rect, const SDL_Rect* textureRect) {
	return SDL_RenderCopy(renderer, texture, textureRect, rect) < 0;
}

bool Window::DrawTexture(SDL_Texture* texture, const SDL_RendererFlip& flip, const SDL_Rect* rect, const SDL_Rect* textureRect, const Vector2i* pivot, const double& angle) {
	return SDL_RenderCopyEx(renderer, texture, textureRect, rect, angle, (SDL_Point*)pivot, flip) < 0;
}

bool Window::DrawText(std::string text, Vector2i pos, TTF_Font* font, SDL_Color textColor) {
	if (text == "") return false;
	// Render Text to Texture
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	// Calculate Text Location
	SDL_Rect textRect = { pos.x, pos.y };
	TTF_SizeText(font, text.c_str(), &textRect.w, &textRect.h);
	// Render Text to Renderer
	if (SDL_RenderCopy(renderer, textTexture, NULL, &textRect) < 0) {
		// Delete Text Texture
		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(textTexture);
		return true;
	}
	// Delete Text Texture
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
	return false;
}

bool Window::DrawFilledText(std::string text, Vector2i pos, TTF_Font* font, SDL_Color textColor, SDL_Color backgroundColor) {
	if (text == "") return false;
	// Render Text to Texture
	SDL_Surface* textSurface = TTF_RenderText_Shaded(font, text.c_str(), textColor, backgroundColor);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	// Calculate Text Location
	SDL_Rect textRect = { pos.x, pos.y };
	TTF_SizeText(font, text.c_str(), &textRect.w, &textRect.h);
	// Render Text to Renderer
	if (SDL_RenderCopy(renderer, textTexture, NULL, &textRect) < 0) {
		// Delete Text Texture
		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(textTexture);
		return true;
	}
	// Delete Text Texture
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
	return false;
}