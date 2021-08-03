#ifndef SCENE_H
#define SCENE_H
#pragma once

#include "EventQueue.h"

class Scene {
	EventQueue events;

public:
	void Update();
	void Render();
};

#endif // !SCENE_H