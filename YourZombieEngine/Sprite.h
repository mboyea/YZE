#ifndef SPRITE_H
#define SPRITE_H
#pragma once

#include "Component.h"
#include <SDL.h>
#include "Vector2i.h"

class Sprite : public Component {
	INIT_COMPONENT;
	SDL_Rect* clipRect;
public:
	SDL_Texture* texture;
	Vector2i dim;
	int depth;
	Sprite();
	Sprite(SDL_Texture* texture);
	Sprite(SDL_Texture* texture, Vector2i dim);
	~Sprite();
	Vector2i GetTextureDim();
	void DisableClip();
	const SDL_Rect* GetClip();
	void SetClipRect(SDL_Rect rect);
	void SetClipPos(Vector2i pos);
	void SetClipDim(Vector2i dim);
	virtual std::set<Resource>& GetRequiredResources(std::set<Resource>& resourcesOut) override;
	virtual std::ostream& Serialize(std::ostream& os) override;
	virtual std::istream& Deserialize(std::istream& is) override;
};

#endif // !SPRITE_H