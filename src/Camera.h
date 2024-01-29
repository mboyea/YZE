#ifndef CAMERA_H
#define CAMERA_H
#pragma once

#include "Component.h"
#include <SDL.h>
#include "Vector2i.h"

class Camera : public Component {
	INIT_COMPONENT;
	SDL_Texture* renderTexture;
public:
	Camera(Vector2i dim = { 320, 240 });
	virtual ~Camera() override;
	SDL_Texture* GetTexture();
	Vector2i GetDim();
	void SetDim(Vector2i dim);
	operator SDL_Texture*() const;
	virtual std::ostream& Serialize(std::ostream& os) override;
	virtual std::istream& Deserialize(std::istream& is) override;
};

#endif // !CAMERA_H