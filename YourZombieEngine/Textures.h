#ifndef TEXTURES_H
#define TEXTURES_H
#pragma once

#include <SDL_image.h>
#include <string>
#include "Vector2i.h"

namespace Textures {
	void Init();
	SDL_Texture* GetDefault();
	// on window resize or alt-tab, all SDL_TEXTUREACCESS_TARGET textures can be lost;
	// fix the default texture via the following function;
	// return whether the texture was successfully replaced
	void FixLostDefault();
	bool Load(std::string filePath);
	void LoadFromDirectory(std::string folderPath);
	bool Unload(std::string key);
	void UnloadAll();
	bool IsTexture(std::string key);
	SDL_Texture* GetTexture(std::string key);
	std::string GetKey(SDL_Texture* texture);
	Vector2i GetDim(SDL_Texture* texture);
}

#endif // !TEXTURES_H