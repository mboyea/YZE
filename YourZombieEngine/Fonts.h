#ifndef FONTS_H
#define FONTS_H
#pragma once

#include <SDL_ttf.h>
#include <string>

namespace Fonts {
	const unsigned int DEFAULT_SIZE = 16;
	bool Init(std::string filePathDefault, unsigned int sizeDefault = DEFAULT_SIZE);
	TTF_Font* GetDefault();
	bool Load(std::string filePath, unsigned int size);
	void LoadFromDirectory(std::string folderPath, unsigned int size);
	bool Unload(std::string key);
	bool Unload(std::string key, unsigned int size);
	void UnloadAll();
	bool IsFont(std::string key);
	bool IsFont(std::string key, unsigned int size);
	TTF_Font* GetFont(std::string key);
	TTF_Font* GetFont(std::string key, unsigned int size);
	std::string GetKey(TTF_Font* font);
	unsigned int GetSizeFromKey(std::string key);
	std::string GetNameFromKey(std::string key);
}

#endif // !FONTS_H