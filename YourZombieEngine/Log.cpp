#include "Log.h"

static bool didUpdate = false;

Debug::GameLog::GameLog(std::string text, LogType type) : text(text), type(type) {
	time = SDL_GetTicks();
}

std::string Debug::GameLog::AsText() {
	return '(' + std::to_string(time) + ") " + text;
}

SDL_Color Debug::GameLog::GetColor() {
	switch (type) {
	case LogType::LOG:
		return Colors::LIGHT_GREY;
	case LogType::WARNING:
		return Colors::YELLOW;
	case LogType::FAULT:
		return Colors::RED;
	case LogType::SUCCESS:
		return Colors::LIME;
	default:
		return Colors::WHITE;
	}
}

void Debug::Log(std::string text, LogType type) {
	Debug::logs.push_back(GameLog(text, type));
	didUpdate = true;
}

bool Debug::DidLogUpdate() {
	if (didUpdate) {
		didUpdate = false;
		return true;
	} else return false;
}