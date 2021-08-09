#include "Scene.h"
#include "Window.h"
#include "Draw.h"

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

	activeScene = nullptr;
}

void Scene::DoCollisions() {
	activeScene = this;

	activeScene = nullptr;
}

void Scene::LateUpdate() {
	activeScene = this;

	activeScene = nullptr;
}

void Scene::Render() {
	activeScene = this;
	// TODO: identify common patterns of this implementation and condense it to smaller & repeated functions
	// TODO: export the cameras and sprites vectors to their respective Systems namespaces.  Edit them on the creation and destruction of cameras and sprites.  Create a way to do the same for every component.
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
	if (!IsEntity(entity)) {
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
	components[type][index] = Components::IDToType(type);
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