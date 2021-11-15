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
	// for every component type
	for (Components::TypeID type = 0; type < Components::GetTypeCount(); type++) {
		for (auto component : components[type]) {
			delete component;
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
	// for every component type
	for (Components::TypeID type = 0; type < Components::GetTypeCount(); type++) {
		// for every component in that type's list
		for (Components::Index index : entities[type]) {

		}
	}
	/*// for every component type
	for (Components::TypeID type = 0; type < Components::GetTypeCount(); type++) {
		// for every component type
		for (size_t i = 0; i < components[type].size(); i++) {
			delete components[type][i];
		}
	}
	// for every entity
	for (Components::TypeID type = 0; type < Components::GetTypeCount(); type++) {

	}
	// for every component type
	for (Components::TypeID type = 0; type < Components::GetTypeCount(); type++) {
		// for every component index (at Entity indexes) for this component type
		for (Components::Index component : entities[type]) {
			// if the component index is valid
			if (component >= 0) {
				// record the required resources of the component at that component index
				components[type][entities[type][component]]->GetRequiredResources(resourcesOut);
			}
		}
	}*/
	return resourcesOut;
}

std::ostream& Scene::Serialize(std::ostream& os) {
	{ // Serialize Scene Version
		os << SCENE_VERSION << '\n';
	}
	{ // Serialize Typemap
		// TODO: serialize a lookup table between component names and type numbers
		os << '\n';
	}
	{ // Serialize Scene Camera
		os << GetEntityWithComponent(activeCamera) << '\n';
	}
	{ // Serialize Required Resources
		std::set<Resource> resources;
		GetRequiredResources(resources);
		int type = 0;
		int index = 0;
		int indexCount = resources.size() - 1;
		for (Resource resource : resources) {
			while (type < resource.type) {
				os << '\n';
				type++;
			}
			os << resource;
			if (index < indexCount) {
				os << ' ';
			}
			index++;
		}
		while (type < MAX_RESOURCE_TYPES - 1) {
			type++;
			os << '\n';
		}
		os << '\n';
	}
	/*
	{ // Serialize Scene Entities
		// for every component type
		for (Components::TypeID type = 0; type < Components::GetTypeCount(); type++) {
			// for every component index of that type on every entity
			for (Components::Index index : entities[type]) {
				
			}
		}
		// for every entity
		for (Entities::Index entity : entities[0]) {
			// for every component
			for (Components::TypeID type = 0; type < Components::GetTypeCount(); type++) {
				// if the entity has that component
				if (entities[type][entity] >= 0) {
					// serialize that component
					components[type][entities[type][entity]]->Serialize(os);
				}
			}
		}
		os << '\n';
	}
	{ // Mark the last line as not an Entity
		os << Components::INVALID_TYPE;
	}
	*/
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

Entities::Index Scene::GetEntityWithComponent(Component* component) {
	const Components::TypeID type = component->GetVirtualTypeID();
	// for every entity
	for (Entities::Index entity : entities[type]) {
		// if the entity has that type of component
		if (entities[type][entity] >= 0) {
			// if the entity has that specific component
			if (components[type][entities[type][entity]] == component) {
				return entity;
			}
		}
	}
	return Entities::INVALID_INDEX;
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