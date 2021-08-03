#include "Audio.h"
#include "Files.h"
#include "Log.h"
#include <unordered_map>

static std::unordered_map<std::string, Mix_Chunk*> audios;

void Audio::Init(int rate, Uint16 format, int channels, int buffers) {
	SDL_Init(SDL_INIT_AUDIO);
	if (Mix_OpenAudio(rate, format, channels, buffers) < 0) {
		Debug::Log("Unable to open audio: " + *Mix_GetError(), WARNING);
	}
}

bool Audio::Load(std::string filePath) {
	if (!Files::DoesPathExist(filePath)) {
		Debug::Log("File \"" + filePath + "\" does not exist.", WARNING);
		Debug::Log("Audio load failed.", FAULT);
		return false;
	}
	Mix_Chunk* audio = Mix_LoadWAV(filePath.c_str());
	if (audio == nullptr) {
		Debug::Log("Audio unable to load: " + *Mix_GetError(), WARNING);
		return false;
	}
	audios[Files::GetFileName(filePath)] = audio;
	return true;
}

void Audio::LoadFromDirectory(std::string folderPath) {
	std::set<std::string> files =
		Files::GetFilesWithExtension(
			Files::GetFilesInDirectory(folderPath),
			"mp3"
		);
	for (std::string file : files) {
		Load(file);
	}
}

bool Audio::Unload(std::string key) {
	if (IsAudio(key)) {
		delete audios[key];
		audios.erase(key);
		return true;
	}
	return false;
}

void Audio::UnloadAll() {
	for (std::pair<std::string, Mix_Chunk*> audio : audios) {
		delete audio.second;
	}
	audios.clear();
}

bool Audio::IsAudio(std::string key) {
	return audios.find(key) != audios.end();
}

Mix_Chunk* Audio::GetAudio(std::string key) {
	if (IsAudio(key)) {
		return audios[key];
	}
	return nullptr;
}

std::string Audio::GetKey(Mix_Chunk* audio) {
	for (auto it = audios.begin(); it != audios.end(); it++) {
		if (it->second == audio) {
			return it->first;
		}
	}
	return "";
}

void Audio::Play(Mix_Chunk* audio) {
	Mix_PlayChannel(-1, audio, 0);
}

bool Audio::Play(std::string key) {
	if (!IsAudio(key)) {
		Debug::Log("Tried to play audio \"" + key + "\" which was not found.");
		return false;
	}
	Play(audios[key]);
	return true;
}