#include "Sprite.h"
REGISTER_COMPONENT(Sprite);
#include "Textures.h"

Sprite::Sprite()
	: texture(Textures::GetDefault()), dim({ 16, 16 }) {}

Sprite::Sprite(SDL_Texture* texture)
	: texture(texture), dim(Textures::GetDim(texture)) {}

Sprite::Sprite(SDL_Texture* texture, Vector2i dim)
	: texture(texture), dim(dim) {}

Vector2i Sprite::GetTextureDim() {
	return Textures::GetDim(texture);
}

std::set<Resource>& Sprite::GetRequiredResources(std::set<Resource>& resourcesOut){
	// TODO: logic
	return resourcesOut;
}

std::ostream& Sprite::Serialize(std::ostream& os) {
	// TODO: logic
	return os;
}

std::istream& Sprite::Deserialize(std::istream& is) {
	// TODO: logic
	return is;
}
