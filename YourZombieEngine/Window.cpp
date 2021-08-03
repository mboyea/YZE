#include "Window.h"
#include "Draw.h"
#include "Log.h"
#include <vector>

static SDL_Texture* renderTarget = nullptr;
static SDL_Rect viewportRect = { 0, 0, 0, 0 };
static Vector2i windowDim = { 0, 0 };
static SDL_Color letterboxColor = Colors::BLACK;

void RecalculateViewportRect() {
	SDL_QueryTexture(renderTarget, NULL, NULL, &viewportRect.x, &viewportRect.y);
	float scale = std::min((float)windowDim.x / viewportRect.x, (float)windowDim.y / viewportRect.y);
	SDL_Point scaledDim = { (int)(viewportRect.x * scale), (int)(viewportRect.y * scale) };
	viewportRect = {
		std::max(0, (windowDim.x - scaledDim.x) / 2),
		std::max(0, (windowDim.y - scaledDim.y) / 2),
		scaledDim.x,
		scaledDim.y
	};
}

void Window::Init(std::string title, Vector2i dim) {
	windowDim = dim;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		Debug::Log("Unable to initialize SDL: " + *SDL_GetError(), FAULT);
		exit(1);
	}
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowDim.x, windowDim.y, SDL_WINDOW_RESIZABLE);
	if (!window) {
		Debug::Log("Unable to open " + std::to_string(dim.x) + "x" + std::to_string(dim.y) + " window " + title + ": " + *SDL_GetError(), FAULT);
		exit(1);
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		Debug::Log("Unable to create renderer: " + *SDL_GetError(), FAULT);
		exit(1);
	}
	SetLetterboxColor(Colors::BLACK);
}

void Window::HandleEvents() {
	std::vector<SDL_Event> eventsIgnored;
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			exit(0);
			break;
		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
				windowDim = { event.window.data1, event.window.data2 };
				RecalculateViewportRect();
				// set render target to the window
				SetDrawTarget(NULL);
				// draw letterbox color
				SetDrawColor(letterboxColor);
				Paint();
				// draw viewport
				DrawTexture(renderTarget, &viewportRect);
				// present the window
				SDL_RenderPresent(renderer);
				// return render target to the viewport
				SetDrawTarget(renderTarget);
			}
			break;
		default:
			// ignore all other events
			eventsIgnored.push_back(event);
			break;
		}
	}
	for (SDL_Event event : eventsIgnored) {
		SDL_PushEvent(&event);
	}
}

void Window::PresentWindow() {
	// set render target to the window
	SetDrawTarget(NULL);
	// draw viewport
	DrawTexture(renderTarget, &viewportRect);
	// present the window
	SDL_RenderPresent(renderer);
	// return render target to the viewport
	SetDrawTarget(renderTarget);
}

void Window::SetLetterboxColor(SDL_Color color) {
	letterboxColor = color;
	// set render target to the window
	SetDrawTarget(NULL);
	// draw letterbox color
	SetDrawColor(letterboxColor);
	Paint();
	// draw viewport
	DrawTexture(renderTarget, &viewportRect);
	// present the window
	SDL_RenderPresent(renderer);
	// return render target to the viewport
	SetDrawTarget(renderTarget);
}

void Window::SetWindowIcon(std::string filePath) {
	SDL_Surface* icon = IMG_Load(filePath.c_str());
	if (!icon) {
		Debug::Log("Unable to load icon from " + filePath + ": " + IMG_GetError(), FAULT);
		return;
	}
	SDL_SetWindowIcon(window, icon);
	SDL_FreeSurface(icon);
}

std::string Window::GetWindowTitle() {
	return SDL_GetWindowTitle(window);
}

void Window::SetWindowTitle(std::string title) {
	SDL_SetWindowTitle(window, title.c_str());
}

Vector2i Window::GetWindowSize() {
	return windowDim;
}

void Window::SetWindowSize(Vector2i dim) {
	SDL_SetWindowSize(window, dim.x, dim.y);
	windowDim = dim;
}

SDL_Texture* Window::GetRenderTarget() {
	return renderTarget;
}

void Window::SetRenderTarget(SDL_Texture* texture) {
	renderTarget = texture;
	RecalculateViewportRect();
	SetDrawTarget(texture);
}

SDL_Rect Window::GetViewportRect() {
	return viewportRect;
}

Vector2i Window::GetViewportPos() {
	return { viewportRect.x, viewportRect.y };
}

Vector2i Window::GetViewportSize() {
	return { viewportRect.w, viewportRect.h };
}