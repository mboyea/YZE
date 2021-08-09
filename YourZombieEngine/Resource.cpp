#include "Resource.h"
std::string Resource::textureFolderPath = "Resources/Textures";
std::string Resource::animationFolderPath = "Resources/Animations";
std::string Resource::fontFolderPath = "Resources/Fonts";
std::string Resource::audioFolderPath = "Resources/Audio";
std::string Resource::textFolderPath = "Resources/Text";
std::string Resource::textureExtension = "png";
std::string Resource::animationExtension = "zanim";
std::string Resource::fontExtension = "ttf";
std::string Resource::audioExtension = "mp3";
std::string Resource::textExtension = "txt";
#include "Files.h"
#include "Textures.h"
// #include "Animations.h"
#include "Fonts.h"
#include "Audio.h"
// #include "Text.h"

bool Resource::operator<(const Resource& rhs) {
	return type < rhs.type || name < rhs.name;
}

std::string Resource::TypeToString() {
	switch (type) {
	case RESOURCE_TEXTURE:
		return "Texture";
	case RESOURCE_ANIMATION:
		return "Animation";
	case RESOURCE_FONT:
		return "Font";
	case RESOURCE_AUDIO:
		return "Audio";
	case RESOURCE_TEXT:
		return "Text";
	case MAX_RESOURCE_TYPES:
		return "MAX_RESOURCE_TYPES";
	default:
		return "ERROR";
	}
}

std::string Resource::GetFilePath() {
	switch (type) {
	case RESOURCE_TEXTURE:
		return Files::ForceFilePath(name, textureFolderPath, textureExtension);
	case RESOURCE_ANIMATION:
		return Files::ForceFilePath(name, animationFolderPath, animationExtension);
	case RESOURCE_FONT:
		return Files::ForceFilePath(Fonts::GetNameFromKey(name), fontFolderPath, fontExtension);
	case RESOURCE_AUDIO:
		return Files::ForceFilePath(name, audioFolderPath, audioExtension);
	case RESOURCE_TEXT:
		return Files::ForceFilePath(name, textFolderPath, textExtension);
	case MAX_RESOURCE_TYPES:
		return Files::ForceFilePath(name, "MAX_RESOURCE_TYPES", "resourceerror");
	default:
		return Files::ForceFilePath(name, "ERROR", "resourceerror");
	}
}
/*
std::ostream& operator<<(std::ostream& os, const Resource& rhs) {
	return os << Files::EncodeString(rhs.name);
}

std::istream& operator>>(std::istream& is, Resource& rhs) {
	rhs.name = Files::DecodeString(is);
	return is;
}
*/
bool LoadResource(Resource resource) {
	std::string filePath = resource.GetFilePath();
	switch (resource.type) {
	case RESOURCE_TEXTURE:
		if (Textures::IsTexture(resource.name)) {
			return true;
		}
		return Textures::Load(filePath);
	case RESOURCE_ANIMATION:
		// TODO: load animations
		return false;
	case RESOURCE_FONT:
		if (Fonts::IsFont(resource.name)) {
			return true;
		}
		return Fonts::Load(filePath, Fonts::GetSizeFromKey(resource.name));
	case RESOURCE_AUDIO:
		if (Audio::IsAudio(resource.name)) {
			return true;
		}
		return Audio::Load(filePath);
	case RESOURCE_TEXT:
		// TODO: load text
		return false;
	case MAX_RESOURCE_TYPES:
		return false;
	default:
		return false;
	}
}

bool LoadResources(std::set<Resource> resources) {
	bool didSucceed = true;
	for (Resource resource : resources) {
		didSucceed = didSucceed && LoadResource(resource);
	}
	return didSucceed;
}