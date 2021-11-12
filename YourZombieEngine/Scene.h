#ifndef SCENE_H
#define SCENE_H
#pragma once

#include "EventQueue.h"
#include "Component.h"
#include "Camera.h"
#include "Entities.h"
#include <SDL.h>
#include <vector>
#include <queue>

#define SCENE_VERSION "0.0.0"

namespace Components {
	typedef short Index;
	const Index NO_COMPONENT = -1;
}

class Scene {
private:
	EventQueue events;
	// std::vector<std::string> entityNames;
	std::vector<Components::Index>* entities;
	std::vector<Component*>* components;
	/* * * * * * * * * * * * * * * * *\
	*          entityNames
	*   |  0  |  1  |  2  |  3  | 
	*---|-----|-----|-----|-----|-
	*   |James|Logan|Jacob|Lucas| 
	*---|-----|-----|-----|-----|-
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
	Camera* activeCamera;
	Scene();
	~Scene();
	void Update();
	void DoCollisions();
	void LateUpdate();
	void Render();
	static Scene* GetActiveScene();
	virtual std::set<Resource>& GetRequiredResources(std::set<Resource>& resourcesOut);
	virtual std::ostream& Serialize(std::ostream& os);
	virtual std::istream& Deserialize(std::istream& is);

	bool IsEntity(Entities::Index entity);
	Entities::Index AddEntity();
	void DeleteEntity(Entities::Index entity);
	Component* AddComponent(Components::TypeID type, Entities::Index entity);
	template<typename T> T* AddComponent(Entities::Index entity) {
		return static_cast<T*>(AddComponent(Components::TypeToID<T>(), entity));
	}
	Component* GetComponent(Components::TypeID type, Entities::Index entity);
	template<typename T> T* GetComponent(Entities::Index entity) {
		return static_cast<T*>(GetComponent(T::GetTypeID(), entity));
	}
	bool HasComponent(Components::TypeID type, Entities::Index entity);
	template<typename T> bool HasComponent(Entities::Index entity) {
		return HasComponent(T::GetTypeID(), entity);
	}
	void DeleteComponent(Components::TypeID type, Entities::Index entity);
	template<typename T> void DeleteComponent(Entities::Index entity) {
		DeleteComponent(T::GetTypeID(), entity);
	}
};

#endif // !SCENE_H