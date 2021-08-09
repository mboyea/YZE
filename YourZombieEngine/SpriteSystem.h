#ifndef SPRITE_SYSTEM_H
#define SPRITE_SYSTEM_H
#pragma once

#include "Scene.h"
#include "Sprite.h"
#include "Transform.h"

namespace Systems {
	namespace Sprites {
		void Render(Sprite* sprite, Transform* spriteTransform);
		void Render(Sprite* sprite, Transform* spriteTransform, Transform* cameraTransform);
	}
}

#endif // !SPRITE_SYSTEM_H