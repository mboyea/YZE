#ifndef SPRITE_H
#define SPRITE_H
#pragma once

#include "Component.h"
#include <SDL.h>
#include "Vector2i.h"

class Sprite : public Component {
	INIT_COMPONENT();
	SDL_Texture* texture;
	Vector2i dim;
public:
	Vector2i GetDim();
	SDL_Texture* GetTexture();
	void SetTexture(SDL_Texture* texture);
};

#endif // !SPRITE_H