#include "SpriteSystem.h"
#include "Draw.h"
#include "Textures.h"

void Systems::Sprites::Render(Sprite* sprite, Transform* spriteTransform) {
	SDL_Rect rect = {
		spriteTransform->pos.x,
		spriteTransform->pos.y,
		sprite->dim.x,
		sprite->dim.y
	};
	Window::DrawTexture(sprite->texture, &rect);
}

void Systems::Sprites::Render(Sprite* sprite, Transform* spriteTransform, Transform* cameraTransform) {
	SDL_Rect rect = {
		spriteTransform->pos.x - cameraTransform->pos.x,
		spriteTransform->pos.y - cameraTransform->pos.y,
		sprite->dim.x,
		sprite->dim.y
	};
	Window::DrawTexture(sprite->texture, &rect);
}