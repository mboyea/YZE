#include "TestScript.h"
REGISTER_SCRIPT(TestScript);
#include "Scene.h"
#include "Input.h"
#include "Transform.h"

void TestScript::Update(Entities::Index entity) {
	Transform* transform = Scene::GetActiveScene()->GetComponent<Transform>(entity);
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

void TestScript::OnCollision(Collision* collision, Entities::Index entity) {

}