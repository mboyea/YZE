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
	}
};
static std::unordered_map<std::string, std::function<std::string(std::vector<std::string>)>> strFuncs = {};

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
			Debug::Log("Function failed.", WARNING);
			return;
		}
		return;
	}
	// test for string function
	if (strFuncs.find(call) != strFuncs.end()) {
		Debug::Log("> " + func, SUCCESS);
		std::string result = strFuncs[call](args);
		if (result == "") {
			Debug::Log("Function failed.", WARNING);
			return;
		}
		Debug::Log("= " + result);
		return;
	}

	Debug::Log("> " + func, WARNING);
	Debug::Log("Unsupported function.");
}

void Debug::AddFunction(std::function<bool(std::vector<std::string>)> func, std::string call) {
	std::transform(call.begin(), call.end(), call.begin(), static_cast<int(*)(int)>(std::tolower));
	boolFuncs[call] = func;
}

void Debug::AddFunction(std::function<std::string(std::vector<std::string>)> func, std::string call) {
	std::transform(call.begin(), call.end(), call.begin(), static_cast<int(*)(int)>(std::tolower));
	strFuncs[call] = func;
}

unsigned int Debug::GetFunctionCount() {
	return boolFuncs.size() + strFuncs.size();
}

void Debug::LogHelp(int pageIndex) {
	int pageCount = (int)std::ceil((float)GetFunctionCount() / pageSize);
	if (pageIndex < 1) {
		pageIndex = 1;
		Debug::Log("Help index out-of-bounds.", WARNING);
	}
	if (pageIndex > pageCount) {
		pageIndex = pageCount;
		Debug::Log("Help index out-of-bounds.", WARNING);
	}

	int i = 0;
	for (auto boolFunc : boolFuncs) {
		if (i < (pageIndex - 1) * pageSize) {
			i++;
			continue;
		}
		if (i >= pageIndex * pageSize) {
			break;
		}
		Debug::Log(boolFunc.first + "(args)");
		i++;
	}
	for (auto strFunc : strFuncs) {
		if (i < (pageIndex - 1) * pageSize) {
			i++;
			continue;
		}
		if (i >= pageIndex * pageSize) {
			break;
		}
		Debug::Log(strFunc.first + "(args)");
		i++;
	}
	Debug::Log("- - - Help(" + std::to_string(pageIndex) + '/' + std::to_string(pageCount) + ") - - -");
}

void Debug::SetHelpLogCount(int lineCount) {
	if (lineCount < 1) {
		pageSize = 1;
		return;
	}
	pageSize = lineCount;
}

std::vector<std::string> GetArgList(std::string& args) {
	std::vector<std::string> argList;
	std::string currentArg = "";

	for (std::string::iterator it = args.begin(); it < args.end(); it++) {
		// ignore whitespace
		while (*it == ' ') {
			it++;
		}
		// push back the arg if a comma is reached
		if (*it == ',') {
			argList.push_back(currentArg);
			currentArg = "";
			continue;
		}
		// add the char to the arg
		currentArg += *it;
	}
	if (currentArg != "") {
		// push back the last arg
		argList.push_back(currentArg);
	}

	return argList;
}

void Debug::InterpretConsoleCommand(std::string* text) {
	if (text->size() < 1) {
		return;
	}

	if ((*text)[0] == '-') {
		Log(text->substr(1, text->size() - 1));
	}
	else if ((*text)[text->size() - 1] == ')') {
		for (std::string::iterator i = text->begin(); i < text->end(); i++) {
			if (*i == '(') {
				std::string name = std::string(text->begin(), i);
				std::string args = std::string(i + 1, text->end() - 1);
				name.erase(remove(name.begin(), name.end(), ' '), name.end());
				std::transform(name.begin(), name.end(), name.begin(), static_cast<int(*)(int)>(std::tolower));
				std::transform(args.begin(), args.end(), args.begin(), static_cast<int(*)(int)>(std::tolower));

				CallFunction(name, GetArgList(args));
				break;
			}
		}
	}
	else {
		Log("Unsupported cmd \"" + *text + "\"", WARNING);
	}

	text->clear();
}

bool IsInteger(const std::string& str) {
	if (str.empty() || (!std::isdigit(str[0]) && str[0] != '-' && str[0] != '+')) {
		return false;
	}
	char* p;
	std::strtol(str.c_str(), &p, 10);
	return (*p == 0);
}

bool ToBool(const std::string& str) {
	if (str[0] == 'T' || str[0] == 't') {
		return true;
	}
	if (IsInteger(str)) {
		return std::stoi(str);
	}
	return false;
}

bool ToString(const bool& boolean) {
	switch (boolean) {
	case true:
		return "True";
	case false:
		return "False";
	}
	return "BoolToString Failed";
}
