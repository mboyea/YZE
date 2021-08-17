#include "Scene.h"
#include "Window.h"
#include "Draw.h"

#include "ColliderSystem.h"
#include "ScriptSystem.h"
#include "SpriteSystem.h"

static Scene* activeScene = nullptr;

Scene::Scene()
	: entities(new std::vector<Components::Index>[Components::GetTypeCount()]),
	components(new std::vector<Component*>[Components::GetTypeCount()]),
	oldComponentIndexes(new std::queue<Components::Index>[Components::GetTypeCount()]),
	activeCamera(nullptr) {}

Scene::~Scene() {
	for (Components::TypeID type = 0; type < Components::GetTypeCount(); type++) {
		for (size_t i = 0; i < components[type].size(); i++) {
			delete components[type][i];
		}
	}
	delete[] entities;
	delete[] components;
}

void Scene::Update() {
	activeScene = this;
	events.HandleEvents();
	for (Component* transform : components[Transform::GetTypeID()]) {
		static_cast<Transform*>(transform)->lastPos = static_cast<Transform*>(transform)->pos;
	}
	for (Component* scriptList : components[ScriptList::GetTypeID()]) {
		for (Script* script : static_cast<ScriptList*>(scriptList)->scripts) {
			script->CallStart(scriptList->entity);
			script->Update(scriptList->entity);
		}
	}
	activeScene = nullptr;
}

void Scene::DoCollisions() {
	activeScene = this;
	// list of relevant colliders and their respective important components
	std::vector<std::tuple<Collider*, Transform*, ScriptList*>> colliders;
	for (Component* colliderList : components[ColliderList::GetTypeID()]) {
		if (entities[Transform::GetTypeID()][colliderList->entity] >= 0) {
			for (Collider* collider : static_cast<ColliderList*>(colliderList)->colliders) {
				colliders.push_back(
					{ collider,
					GetComponent<Transform>(colliderList->entity),
					GetComponent<ScriptList>(colliderList->entity) }
				);
			}
		}
	}
	for (auto lhs = colliders.begin(); lhs < colliders.end(); lhs++) {
		for (auto rhs = lhs + 1; rhs < colliders.end(); rhs++) {
			const Collision c = std::get<0>(*lhs)->DoCollision(
				std::get<1>(*lhs),
				std::get<0>(*rhs),
				std::get<1>(*rhs)
			);
			if (c.didCollide) {
				if (std::get<2>(*lhs)) {
					for (Script* script : std::get<2>(*lhs)->scripts) {
						script->OnCollision(std::get<1>(*lhs)->entity, std::get<1>(*rhs)->entity, c.impulse.first);
					}
				}
				if (std::get<2>(*rhs)) {
					for (Script* script : std::get<2>(*rhs)->scripts) {
						script->OnCollision(std::get<1>(*rhs)->entity, std::get<1>(*lhs)->entity, c.impulse.second);
					}
				}
			}
		}
	}
	activeScene = nullptr;
}

void Scene::LateUpdate() {
	activeScene = this;
	for (Component* scriptList : components[ScriptList::GetTypeID()]) {
		for (Script* script : static_cast<ScriptList*>(scriptList)->scripts) {
			script->LateUpdate(scriptList->entity);
		}
	}
	activeScene = nullptr;
}

void Scene::Render() {
	activeScene = this;
	// TODO: CLEAN UP Render() FUNCTION
	// TODO: IMPLEMENT depth VARIABLE
	std::vector<Entities::Index> cameras;
	{ // Get scene cameras
		Entities::Index entity = 0;
		for (Components::Index index : entities[Camera::GetTypeID()]) {
			if (index >= 0) {
				if (entities[Transform::GetTypeID()][entity] >= 0) {
					cameras.push_back(entity);
				}
			}
			entity++;
		}
	}
	std::vector<Entities::Index> sprites;
	{ // Get scene sprites
		Entities::Index entity = 0;
		for (Components::Index index : entities[Sprite::GetTypeID()]) {
			if (index >= 0) {
				if (entities[Transform::GetTypeID()][entity] >= 0) {
					sprites.push_back(entity);
				}
			}
			entity++;
		}
	}
	// Render to each camera
	for (Entities::Index camera : cameras) {
		Window::SetDrawTarget(GetComponent<Camera>(camera)->GetTexture());
		// render each sprite
		for (Entities::Index sprite : sprites) {
			Systems::Sprites::Render(
				GetComponent<Sprite>(sprite),
				GetComponent<Transform>(sprite),
				GetComponent<Transform>(camera)
			);
		}
		Window::SetDrawTarget(Window::GetRenderTarget());
	}
	activeScene = nullptr;
}

Scene* Scene::GetActiveScene() {
	return activeScene;
}

std::set<Resource>& Scene::GetRequiredResources(std::set<Resource>& resourcesOut) {
	// TODO: determine required resources
	return resourcesOut;
}

std::ostream& Scene::Serialize(std::ostream& os) {
	// TODO: serialize scene
	return os;
}

std::istream& Scene::Deserialize(std::istream& is) {
	// TODO: deserialize scene
	return is;
}

bool Scene::IsEntity(Entities::Index entity) {
	return entity >= 0 && entity < (Entities::Index)entities[0].size();
}

Entities::Index Scene::AddEntity() {
	Entities::Index id;
	if (oldEntityIndexes.empty()) {
		id = entities[0].size();
		for (Components::TypeID type = 0; type < Components::GetTypeCount(); type++) {
			entities[type].push_back(Components::NO_COMPONENT);
		}
	} else {
		id = oldEntityIndexes.front();
		oldEntityIndexes.pop();
	}
	return id;
}

void Scene::DeleteEntity(Entities::Index entity) {
	if (!IsEntity(entity)) {
		return;
	}
	for (Components::TypeID type = 0; type < Components::GetTypeCount(); type++) {
		if (entities[type][entity] > 1) {
			DeleteComponent(type, entity);
		}
	}
}

Component* Scene::AddComponent(Components::TypeID type, Entities::Index entity) {
	// if component already exists, return it
	Components::Index index = entities[type][entity];
	if (index >= 0) {
		return components[type][index];
	}
	// generate new component
	if (oldComponentIndexes[type].empty()) {
		index = (Components::Index)components[type].size();
		components[type].resize(index + 1, nullptr);
	} else {
		index = oldComponentIndexes[type].front();
		oldComponentIndexes[type].pop();
	}
	entities[type][entity] = index;
	components[type][index] = Components::IDToType(type);
	components[type][index]->entity = entity;
	return components[type][index];
}

Component* Scene::GetComponent(Components::TypeID type, Entities::Index entity) {
	return components[type][entities[type][entity]];
}

bool Scene::HasComponent(Components::TypeID type, Entities::Index entity) {
	return entities[type][entity] >= 0;
}

void Scene::DeleteComponent(Components::TypeID type, Entities::Index entity) {
	Components::Index index = entities[type][entity];
	if (index < 0 || index >= (Components::Index)components[type].size()) {
		return;
	}
	delete components[type][index];
	components[type][index] = nullptr;
	entities[type][entity] = Components::NO_COMPONENT;
	oldComponentIndexes[type].push(index);
}