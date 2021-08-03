#include <SDL.h>
#include "Textures.h"
#include "Window.h"
#include "Draw.h"
#include "Files.h"
#include "Log.h"
#include <unordered_map>
#include <unordered_set>
#include "Vector2i.h"

static SDL_Texture* defaultTexture = nullptr;
static std::unordered_map<std::string, SDL_Texture*> textures;

bool SetDefault(SDL_Texture* texture) {
	if (texture) {
		defaultTexture = texture;
		textures[""] = texture;
		return true;
	}
	return false;
}

void Textures::Init() {
	// enable .png loading
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

	// turn off texture blurring
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

	// generate default texture
	const Vector2i TEXTURE_DIM = { 2, 2 };
	SDL_Texture* defaultTexture = SDL_CreateTexture(Window::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, TEXTURE_DIM.x, TEXTURE_DIM.y);
	Window::SetDrawTarget(defaultTexture);
	Window::SetDrawColor(Colors::BLACK);
	Window::DrawFilledRect({ 0, 0, TEXTURE_DIM.x, TEXTURE_DIM.y });
	Window::SetDrawColor(Colors::MAGENTA);
	Window::DrawFilledRect({ 0, 0, TEXTURE_DIM.x / 2, TEXTURE_DIM.y / 2 });
	Window::DrawFilledRect({ TEXTURE_DIM.x / 2, TEXTURE_DIM.y / 2, TEXTURE_DIM.x / 2, TEXTURE_DIM.y / 2 });
	Window::SetDrawTarget(Window::GetRenderTarget());
	SetDefault(defaultTexture);
}

SDL_Texture* Textures::GetDefault() {
	return defaultTexture;
}

bool Textures::Load(std::string filePath) {
	if (!Files::DoesPathExist(filePath)) {
		Debug::Log("File \"" + filePath + "\" does not exist.", WARNING);
		Debug::Log("Texture load failed.", FAULT);
		return false;
	}
	SDL_Surface* surf = IMG_Load(filePath.c_str());
	if (!surf) {
		Debug::Log("Image \"" + filePath + "\" unable to load: " + *IMG_GetError(), WARNING);
		return false;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(Window::renderer, surf);
	if (!texture) {
		Debug::Log("Texture unable to load: " + (std::string)SDL_GetError(), WARNING);
		SDL_FreeSurface(surf);
		return false;
	}
	SDL_FreeSurface(surf);
	textures[Files::GetFileName(filePath)] = texture;
	return true;
}

void Textures::LoadFromDirectory(std::string folderPath) {
	std::set<std::string> files =
		Files::GetFilesWithExtension(
			Files::GetFilesInDirectory(folderPath),
			"png"
		);
	for (std::string file : files) {
		Load(file);
	}
}

bool Textures::Unload(std::string key) {
	if (key != "" && IsTexture(key)) {
		SDL_DestroyTexture(textures[key]);
		textures.erase(key);
		return true;
	}
	return false;
}

void Textures::UnloadAll() {
	for (std::pair<std::string, SDL_Texture*> texture : textures) {
		if (texture.first != "") {
			SDL_DestroyTexture(texture.second);
		}
	}
	textures.clear();
	textures[""] = defaultTexture;
}

bool Textures::IsTexture(std::string key) {
	return textures.find(key) != textures.end();
}

SDL_Texture* Textures::GetTexture(std::string key) {
	if (IsTexture(key)) {
		return textures[key];
	}
	return defaultTexture;
}

std::string Textures::GetKey(SDL_Texture* texture) {
	for (auto it = textures.begin(); it != textures.end(); it++) {
		if (it->second == texture) {
			return it->first;
		}
	}
	return "";
}