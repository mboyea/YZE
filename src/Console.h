#ifndef CONSOLE_H
#define CONSOLE_H
#pragma once

#include <string>
#include <vector>
#include <functional>

namespace Debug {
	inline std::string consoleInput = "";

	bool IsFunction(std::string call);
	void CallFunction(std::string call, std::vector<std::string> args);
	void AddFunction(std::function<bool(std::vector<std::string>)> func, std::string call);
	void AddFunction(std::function<std::string(std::vector<std::string>)> func, std::string call);
	unsigned int GetFunctionCount();
	void LogHelp(int pageIndex = 1);
	void SetHelpLogCount(int lineCount);
	void InterpretConsoleCommand(std::string* text = &consoleInput);
}
bool IsInteger(const std::string& str);
bool ToBool(const std::string& str);
bool ToString(const bool& boolean);

#endif // !CONSOLE_H