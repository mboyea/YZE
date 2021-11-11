#include "Sprite.h"
REGISTER_COMPONENT(Sprite);
#include "SpriteSystem.h"
#include "Textures.h"

Sprite::Sprite()
	: texture(Textures::GetDefault()), dim({ 16, 16 }), clipRect(nullptr), depth(0) {}

Sprite::Sprite(SDL_Texture* texture)
	: texture(texture), dim(Textures::GetDim(texture)), clipRect(nullptr), depth(0) {}

Sprite::Sprite(SDL_Texture* texture, Vector2i dim)
	: texture(texture), dim(dim), clipRect(nullptr), depth(0) {}

Sprite::~Sprite() {
	delete clipRect;
}

Vector2i Sprite::GetTextureDim() {
	return Textures::GetDim(texture);
}

void Sprite::DisableClip() {
	delete clipRect;
	clipRect = nullptr;
}

const SDL_Rect* Sprite::GetClip() {
	return clipRect;
}

void Sprite::SetClipRect(SDL_Rect rect) {
	if (clipRect) {
		clipRect->x = rect.x;
		clipRect->y = rect.y;
		clipRect->w = rect.w;
		clipRect->h = rect.h;
		return;
	}
	clipRect = new SDL_Rect({ rect.x, rect.y, rect.w, rect.h });
}

void Sprite::SetClipPos(Vector2i pos) {
	if (clipRect) {
		clipRect->x = pos.x;
		clipRect->y = pos.y;
		return;
	}
	clipRect = new SDL_Rect({ pos.x, pos.y, 0, 0 });
}

void Sprite::SetClipDim(Vector2i dim) {
	if (clipRect) {
		clipRect->w = dim.x;
		clipRect->h = dim.y;
		return;
	}
	clipRect = new SDL_Rect({ 0, 0, dim.x, dim.y });
}

std::set<Resource>& Sprite::GetRequiredResources(std::set<Resource>& resourcesOut){
	Component::GetRequiredResources(resourcesOut);
	std::string key = Textures::GetKey(texture);
	if (key != "") {
		resourcesOut.insert(Resource(key, RESOURCE_TEXTURE));
	}
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