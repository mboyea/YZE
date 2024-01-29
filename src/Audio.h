#ifndef AUDIO_H
#define AUDIO_H
#pragma once

#include <SDL_mixer.h>
#include <string>

namespace Audio {
	void Init(int rate = 44100, Uint16 format = AUDIO_S16SYS, int channels = 2, int buffers = 4096);
	bool Load(std::string filePath);
	void LoadFromDirectory(std::string folderPath);
	bool Unload(std::string key);
	void UnloadAll();
	bool IsAudio(std::string key);
	Mix_Chunk* GetAudio(std::string key);
	std::string GetKey(Mix_Chunk* audio);
	void Play(Mix_Chunk* audio);
	bool Play(std::string key);
	// TODO:
	// LoadMusic(std::string filePath);
	// LoadMusicFromDirectory(std::string folderPath);
	// UnloadMusic(std::string folderPath);
	// UnloadAllMusic();
	// IsMusic(std::string key);
	// GetMusic(std::string key);
	// GetKey(Music* music);
	// PlayMusic(Music* music);
	// PlayMusic(std::string key);
}

#endif // !AUDIO_H