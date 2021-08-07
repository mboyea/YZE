#include "Scene.h"

static Scene* activeScene = nullptr;

Scene::Scene()
	: entities(new std::vector<Components::Index>[Components::GetTypeCount()]),
	components(new std::vector<Component*>[Components::GetTypeCount()]),
	oldComponentIndexes(new std::queue<Components::Index>[Components::GetTypeCount()]),
	cameraEntity(-1) {}

Scene::~Scene() {
	for (size_t c = 0; c < Components::GetTypeCount(); c++) {
		for (size_t i = 0; i < components[c].size(); i++) {
			delete components[c][i];
		}
	}
	delete[] entities;
	delete[] components;
}

void Scene::Update() {
	activeScene = this;
	events.HandleEvents();

	activeScene = nullptr;
}

void Scene::DoPhysics() {}

void Scene::LateUpdate() {}

void Scene::Render() {
	activeScene = this;

	activeScene = nullptr;
}

Scene* Scene::GetActiveScene() {
	return activeScene;
}

Entities::Index Scene::AddEntity() {
	Entities::Index id;
	if (oldEntityIndexes.empty()) {
		id = entities[0].size();
		for (size_t i = 0; i < Components::GetTypeCount(); i++) {
			entities[i].resize(id + 1, Components::NO_COMPONENT);
		}
	} else {
		id = oldEntityIndexes.front();
		oldEntityIndexes.pop();
	}
	return id;
}

void Scene::DeleteEntity(Entities::Index entity) {
	if (entity < 0 || entity > (Entities::Index)entities[0].size()) {
		return;
	}
	for (size_t i = 0; i < Components::GetTypeCount(); i++) {
		if (entities[i][entity] > 1) {
			DeleteComponent(i, entity);
		}
	}
}

Component* Scene::AddComponent(Components::ID type, Entities::Index entity) {
	if (entity < 0 || entity > (Entities::Index)entities[type].size()) {
		return nullptr;
	}
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
	components[type][index] = Components::IDToType(entity);
	return components[type][index];
}

Component* Scene::GetComponent(Components::ID type, Entities::Index entity) {
	if (entity < 0 || entity > (Entities::Index)entities[type].size()) {
		return nullptr;
	}
	Components::Index index = entities[type][entity];
	if (index >= 0) {
		return components[type][index];
	}
	return nullptr;
}

bool Scene::HasComponent(Components::ID type, Entities::Index entity) {
	if (entity < 0 || entity > (Entities::Index)entities[type].size()) {
		return false;
	}
	return entities[type][entity] >= 0;
}

void Scene::DeleteComponent(Components::ID type, Entities::Index entity) {
	if (entity < 0 || entity > (Entities::Index)entities[type].size()) {
		return;
	}
	Components::Index index = entities[type][entity];
	if (index < 0 || index >= (Components::Index)components[type].size()) {
		return;
	}
	delete components[type][index];
	components[type][index] = nullptr;
	entities[type][entity] = Components::NO_COMPONENT;
	oldComponentIndexes[type].push(index);
}