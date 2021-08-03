#ifndef LOG_H
#define LOG_H
#pragma once

#include <SDL.h>
#include <string>
#include <vector>
#include "Colors.h"

enum LogType {
	LOG = 0,
	WARNING,
	FAULT,
	SUCCESS
};

namespace Debug {
	struct GameLog {
		Uint32 time;
		std::string text;
		LogType type;

		GameLog(std::string text, LogType type);
		std::string AsText();
		SDL_Color GetColor();
	};

	inline std::vector<GameLog> logs;

	void Log(std::string text, LogType type = LOG);
	// returns true if the log updated since the last time this function was called
	bool DidLogUpdate();
}

#endif // !LOG_H