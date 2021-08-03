#ifndef TEXTURES_H
#define TEXTURES_H
#pragma once

#include <SDL_image.h>
#include <string>

namespace Textures {
	void Init();
	SDL_Texture* GetDefault();
	bool Load(std::string filePath);
	void LoadFromDirectory(std::string folderPath);
	bool Unload(std::string key);
	void UnloadAll();
	bool IsTexture(std::string key);
	SDL_Texture* GetTexture(std::string key);
	std::string GetKey(SDL_Texture* texture);
}

#endif // !TEXTURES_H