#include "TestScript.h"
REGISTER_SCRIPT(TestScript);
#include "Scene.h"
#include "Input.h"

void TestScript::Start(Entities::Index entity) {
	transform = Scene::GetActiveScene()->AddComponent<Transform>(entity);
}

void TestScript::Update(Entities::Index entity) {
	if (Input::IsKeyDown(SDL_SCANCODE_UP)) {
		transform->pos.y--;
	}
	if (Input::IsKeyDown(SDL_SCANCODE_DOWN)) {
		transform->pos.y++;
	}
	if (Input::IsKeyDown(SDL_SCANCODE_LEFT)) {
		transform->pos.x--;
	}
	if (Input::IsKeyDown(SDL_SCANCODE_RIGHT)) {
		transform->pos.x++;
	}
}

void TestScript::OnCollision(Entities::Index entity, Entities::Index rhs, Vector2i impulse) {
	// transform->pos = { 64, 64 };
}