#include "Input.h"
#include <bitset>
#include <vector>
#include "Window.h"

const unsigned short MAX_SCANCODE = 285U;
typedef std::bitset<MAX_SCANCODE> KeyboardState;

static std::string* ioTextTarget = nullptr;
static bool didTextIOUpdate = false;
static char ignoreTextChar = '\n';
static KeyboardState currentKeyState;
static KeyboardState lastKeyState;
Vector2i currentMousePos = { 0, 0 };

void UpdateKey(SDL_KeyboardEvent* event, bool isDown) {
	if (event->repeat == 0 && event->keysym.scancode < MAX_SCANCODE) {
		currentKeyState[event->keysym.scancode] = isDown;
	}
}

void ClearInputBufferText() {
	std::vector<SDL_Event> eventsIgnored;
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_TEXTINPUT:
			// throw away event
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

void HandleEventsStandard() {
	std::vector<SDL_Event> eventsIgnored;
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			// update new keyboard state for each key
			UpdateKey(&event.key, true);
			break;
		case SDL_KEYUP:
			// update new keyboard state for each key
			UpdateKey(&event.key, false);
			break;
		case SDL_MOUSEMOTION:
			currentMousePos = { event.motion.x, event.motion.y };
		/*
		case SDL_MOUSEBUTTONDOWN:
			// https://wiki.libsdl.org/SDL_MouseButtonEvent
		case SDL_MOUSEBUTTONUP:
			// https://wiki.libsdl.org/SDL_MouseButtonEvent
		case SDL_MOUSEWHEEL:
			// https://wiki.libsdl.org/SDL_MouseWheelEvent
		*/
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

void HandleEventsTextIO() {
	std::vector<SDL_Event> eventsIgnored;
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			// Backspace
			if (event.key.keysym.sym == SDLK_BACKSPACE && ioTextTarget->size() > 0) {
				didTextIOUpdate = true;
				ioTextTarget->pop_back();
			}
			// Copy Buffer Interactions
			else if (SDL_GetModState() & KMOD_CTRL) {
				// Copy
				if (event.key.keysym.sym == SDLK_c) {
					didTextIOUpdate = true;
					SDL_SetClipboardText(ioTextTarget->c_str());
				}
				// Paste
				else if (event.key.keysym.sym == SDLK_v) {
					didTextIOUpdate = true;
					*ioTextTarget += SDL_GetClipboardText();
				}
				// Cut
				else if (event.key.keysym.sym == SDLK_x) {
					didTextIOUpdate = true;
					SDL_SetClipboardText(ioTextTarget->c_str());
					ioTextTarget->erase();
				}
			}
			UpdateKey(&event.key, true);
			break;
		case SDL_KEYUP:
			UpdateKey(&event.key, false);
			break;
		case SDL_MOUSEMOTION:
			currentMousePos = { event.motion.x, event.motion.y };
		case SDL_TEXTINPUT:
			// Text Input
			if (!(event.text.text[0] == ignoreTextChar || SDL_GetModState() & KMOD_CTRL && (event.text.text[0] == 'c' || event.text.text[0] == 'C' || event.text.text[0] == 'v' || event.text.text[0] == 'V'))) {
				didTextIOUpdate = true;
				*ioTextTarget += event.text.text;
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

void Input::HandleEvents() {
	// record last keyboard state
	lastKeyState = currentKeyState;

	if (ioTextTarget == nullptr) {
		HandleEventsStandard();
	} else HandleEventsTextIO();
}

bool Input::IsKeyDown(SDL_Scancode key) {
	return currentKeyState[key];
}

bool Input::IsKeyPressed(SDL_Scancode key) {
	return currentKeyState[key] && !lastKeyState[key];
}

Vector2i Input::GetMouseWindowPos() {
	return currentMousePos;
}

Vector2i Input::GetMouseViewportPos() {
	return currentMousePos - Window::GetViewportPos();
}

void Input::StartTextInput(std::string* target, char ignoreChar) {
	SDL_StartTextInput();
	ClearInputBufferText();
	ioTextTarget = target;
	ignoreTextChar = ignoreChar;
	didTextIOUpdate = true;
}

void Input::StopTextInput() {
	SDL_StopTextInput();
	ioTextTarget = nullptr;
}

bool Input::DidTextIOUpdate() {
	if (didTextIOUpdate) {
		didTextIOUpdate = false;
		return true;
	}
	else return false;
}