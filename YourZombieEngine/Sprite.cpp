#include "Sprite.h"
REGISTER_COMPONENT(Sprite);

Vector2i Sprite::GetDim() {
	return Vector2i();
}

SDL_Texture* Sprite::GetTexture() {
	return nullptr;
}

void Sprite::SetTexture(SDL_Texture* texture) {
}
