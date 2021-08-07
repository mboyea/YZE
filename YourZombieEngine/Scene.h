#ifndef SCENE_H
#define SCENE_H
#pragma once

#include "EventQueue.h"
#include "Component.h"
#include "Camera.h"
#include <SDL.h>
#include <vector>
#include <queue>

namespace Entities {
	typedef int Index;
}

namespace Components {
	typedef short Index;
	const Index NO_COMPONENT = -1;
}

class Scene {
private:
	EventQueue events;
	std::vector<Components::Index>* entities;
	std::vector<Component*>* components;
	/* * * * * * * * * * * * * * * * *\
	*              entities
	* component| 0 | 1 | 2 | 3 | 
	* types ---|---|---|---|---|-
	*        0 | 0 | ~ | 1 | 2 | 
	*       ---|---|---|---|---|-
	*        1 | 0 | ~ | ~ | 1 | 
	*       ---|---|---|---|---|-
	*        2 | ~ | ~ | ~ | 0 | 
	*       ---|---|---|---|---|-
	*        3 | ~ | 0 | ~ | 1 | 
	*       ---|---|---|---|---|-
	*             components
	* component| 0 | 1 | 2 | 3 | 
	* types ---|---|---|---|---|-
	*        0 | X | X | X | ~ | 
	*       ---|---|---|---|---|-
	*        1 | X | X | ~ | ~ | 
	*       ---|---|---|---|---|-
	*        2 | X | ~ | ~ | ~ | 
	*       ---|---|---|---|---|-
	*        3 | X | X | ~ | ~ | 
	*       ---|---|---|---|---|-
	\* * * * * * * * * * * * * * * * */
	// queue of deleted entity locations for reuse
	std::queue<Entities::Index> oldEntityIndexes;
	// queue of deleted component locations for reuse
	std::queue<Components::Index>* oldComponentIndexes;
public:
	Entities::Index cameraEntity;
	Scene();
	~Scene();
	void Update();
	void DoPhysics();
	void LateUpdate();
	void Render();
	static Scene* GetActiveScene();

	Entities::Index AddEntity();
	void DeleteEntity(Entities::Index entity);
	Component* AddComponent(Components::ID type, Entities::Index entity);
	template<typename T> T* AddComponent(Entities::Index entity) {
		Components::ID type = Components::TypeToID<T>();
		if (entity < 0 || entity > (Entities::Index)entities[type].size()) {
			return nullptr;
		}
		// if component already exists, return it
		Components::Index index = entities[type][entity];
		if (index >= 0) {
			return static_cast<T*>(components[type][index]);
		}
		// generate new component
		if (oldComponentIndexes[type].empty()) {
			index = (Components::Index)components[type].size();
			components[type].resize(index + 1, nullptr);
		}
		else {
			index = oldComponentIndexes[type].front();
			oldComponentIndexes[type].pop();
		}
		entities[type][entity] = index;
		components[type][index] = new T();
		return static_cast<T*>(components[type][index]);
	}
	Component* GetComponent(Components::ID type, Entities::Index entity);
	template<typename T> T* GetComponent(Entities::Index entity) {
		return static_cast<T*>(GetComponent(T::GetTypeID(), entity));
	}
	bool HasComponent(Components::ID type, Entities::Index entity);
	template<typename T> bool HasComponent(Entities::Index entity) {
		return HasComponent(T::GetTypeID(), entity);
	}
	void DeleteComponent(Components::ID type, Entities::Index entity);
	template<typename T> void DeleteComponent(Entities::Index entity) {
		DeleteComponent(T::GetTypeID(), entity);
	}
};

#endif // !SCENE_H