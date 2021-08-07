#ifndef RESOURCE_H
#define RESOURCE_H
#pragma once

#include <string>
#include <set>

enum ResourceType {
	RESOURCE_TEXTURE,
	RESOURCE_ANIMATION,
	RESOURCE_FONT,
	RESOURCE_AUDIO,
	RESOURCE_TEXT,
	MAX_RESOURCE_TYPES
};

struct Resource {
	static std::string textureFolderPath;
	static std::string animationFolderPath;
	static std::string fontFolderPath;
	static std::string audioFolderPath;
	static std::string textFolderPath;
	static std::string textureExtension;
	static std::string animationExtension;
	static std::string fontExtension;
	static std::string audioExtension;
	static std::string textExtension;
	std::string name;
	ResourceType type;
	Resource(std::string name, ResourceType type)
		: name(name), type(type) {}
	bool operator<(const Resource& rhs);
	friend std::ostream& operator<<(std::ostream& os, const Resource& rhs);
	friend std::istream& operator>>(std::istream& is, Resource& rhs);
	std::string TypeToString();
	std::string GetFilePath();
};

bool LoadResource(Resource resource);
bool LoadResources(std::set<Resource> resources);

#endif // !RESOURCE_H