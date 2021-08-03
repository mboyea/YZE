#include <SDL.h>
#include "Fonts.h"
#include "Files.h"
#include "Log.h"
#include <unordered_map>
#include <unordered_set>

TTF_Font* defaultFont = nullptr;
inline std::unordered_map<std::string, TTF_Font*> fonts;

bool SetDefault(TTF_Font* font) {
	if (!font) {
		Debug::Log("Default font unable to load: " + *TTF_GetError(), WARNING);
		return false;
	}
	defaultFont = font;
	return true;
}

bool Fonts::Init(std::string filePathDefault, int sizeDefault) {
	TTF_Init();

	return SetDefault(TTF_OpenFont(filePathDefault.c_str(), sizeDefault));
}

TTF_Font* Fonts::GetDefault() {
	return defaultFont;
}

bool Fonts::Load(std::string filePath, int size) {
	if (!Files::DoesPathExist(filePath)) {
		Debug::Log("File \"" + filePath + "\" does not exist.", WARNING);
		Debug::Log("Font load failed.", FAULT);
		return false;
	}
	TTF_Font* font = TTF_OpenFont(filePath.c_str(), size);
	if (!font) {
		Debug::Log("Font \"" + filePath + "\" at size " + std::to_string(size) + " unable to load: " + (std::string)TTF_GetError(), WARNING);
		return false;
	}
	fonts[Files::GetFileName(filePath) + '_' + std::to_string(size)] = font;
	return true;
}

void Fonts::LoadFromDirectory(std::string folderPath, int size) {
	std::set<std::string> files =
		Files::GetFilesWithExtension(
			Files::GetFilesInDirectory(folderPath),
			"ttf"
		);
	for (std::string file : files) {
		Load(file, size);
	}
}

bool Fonts::Unload(std::string key) {
	if (!IsFont(key)) {
		return false;
	}
	TTF_CloseFont(fonts[key]);
	fonts.erase(key);
	return true;
}

bool Fonts::Unload(std::string key, int size) {
	return Unload(key + '_' + std::to_string(size));
}

void Fonts::UnloadAll() {
	for (std::pair<std::string, TTF_Font*> font : fonts) {
		TTF_CloseFont(font.second);
	}
	fonts.clear();
}

bool Fonts::IsFont(std::string key) {
	return fonts.find(key) != fonts.end();
}

bool Fonts::IsFont(std::string key, int size) {
	return IsFont(key + '_' + std::to_string(size));
}

TTF_Font* Fonts::GetFont(std::string key) {
	if (IsFont(key)) {
		return fonts[key];
	}
	return defaultFont;
}

TTF_Font* Fonts::GetFont(std::string key, int size) {
	return GetFont(key + '_' + std::to_string(size));
}

std::string Fonts::GetKey(TTF_Font* font) {
	for (auto it = fonts.begin(); it != fonts.end(); it++) {
		if (it->second == font) {
			return it->first;
		}
	}
	return "";
}