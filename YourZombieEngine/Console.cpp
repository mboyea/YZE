#include "Console.h"
#include "Log.h"
#include <unordered_map>
#include <algorithm>
#include <SDL.h>

static Uint8 pageSize = 10;
static std::unordered_map<std::string, std::function<bool(std::vector<std::string>)>> boolFuncs = {
	{
		"help",
		[](std::vector<std::string> args) {
			if (args.size() == 1 && IsInteger(args[0])) {
				Debug::LogHelp(std::stoi(args[0]));
				return true;
			}
			Debug::LogHelp();
			return true;
		}
	},
	{
		"sethelppagesize",
		[](std::vector<std::string> args) {
			if (args.size() == 1 && IsInteger(args[0])) {
				Debug::SetHelpLogCount(std::stoi(args[0]));
				return true;
			}
			return false;
		}
	},
	{
		"setdorendercolliders",
		[](std::vector<std::string> args) {
			return false;
		}
	},
	{
		"getdorendercolliders",
		[](std::vector<std::string> args) {
			return false;
		}
	}
};
static std::unordered_map<std::string, std::function<std::string(std::vector<std::string>)>> strFuncs;

bool Debug::IsFunction(std::string call) {
	return boolFuncs.find(call) != boolFuncs.end() || strFuncs.find(call) != strFuncs.end();
}

void Debug::CallFunction(std::string call, std::vector<std::string> args) {
	// get function title
	std::string func = call + '(';
	for (std::string arg : args) {
		func += arg + ", ";
	}
	func.pop_back();
	func[func.size() - 1] = ')';
	// test for bool function
	if (boolFuncs.find(call) != boolFuncs.end()) {
		Debug::Log("> " + func, SUCCESS);
		if (!boolFuncs[call](args)) {
			Log("Function failed.", WARNING);
			return;
		}
		return;
	}
	// test for string function
	if (strFuncs.find(call) != strFuncs.end()) {
		Log("> " + func, SUCCESS);
		std::string result = strFuncs[call](args);
		if (result == "") {
			Log("Function failed.", WARNING);
			return;
		}
		Log("= " + result);
		return;
	}

	Log("> " + func, WARNING);
	Log("Unsupported function.");
}

void Debug::AddFunction(std::function<bool(std::vector<std::string>)> func, std::string call) {
}

void Debug::AddFunction(std::function<std::string(std::vector<std::string>)> func, std::string call) {
}

void Debug::LogHelp(int pageIndex) {
}

void Debug::SetHelpLogCount(int lineCount) {
}

void Debug::InterpretConsoleCommand(std::string* text) {
}

bool IsInteger(const std::string& str) {
	return false;
}

bool ToBool(const std::string& str) {
	return false;
}

bool ToString(const bool& boolean) {
	return false;
}
