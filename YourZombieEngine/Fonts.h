#ifndef FONTS_H
#define FONTS_H
#pragma once

#include <SDL_ttf.h>
#include <string>

namespace Fonts {
	bool Init(std::string filePathDefault, int sizeDefault = 16);
	TTF_Font* GetDefault();
	bool Load(std::string filePath, int size);
	void LoadFromDirectory(std::string folderPath, int size);
	bool Unload(std::string key);
	bool Unload(std::string key, int size);
	void UnloadAll();
	bool IsFont(std::string key);
	bool IsFont(std::string key, int size);
	TTF_Font* GetFont(std::string key);
	TTF_Font* GetFont(std::string key, int size);
	std::string GetKey(TTF_Font* font);
}

#endif // !FONTS_H