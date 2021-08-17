#include "PlayerScript.h"
REGISTER_SCRIPT(PlayerScript);
#include "Scene.h"
#include "Input.h"

void PlayerScript::Start(Entities::Index entity) {
	transform = Scene::GetActiveScene()->AddComponent<Transform>(entity);
	transform->pos = { 0, 0 };
}

void PlayerScript::Update(Entities::Index entity) {
	if (Input::IsKeyDown(SDL_SCANCODE_W)) {
		transform->pos.y--;
	}
	if (Input::IsKeyDown(SDL_SCANCODE_S)) {
		transform->pos.y++;
	}
	if (Input::IsKeyDown(SDL_SCANCODE_A)) {
		transform->pos.x--;
	}
	if (Input::IsKeyDown(SDL_SCANCODE_D)) {
		transform->pos.x++;
	}
}

void PlayerScript::OnCollision(Entities::Index entity, Entities::Index rhs, Vector2i impulse) {
	// transform->pos = { 0, 0 };
}