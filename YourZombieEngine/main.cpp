#include "Window.h"
#include "Draw.h"
#include "Textures.h"
#include "Fonts.h"
#include "Audio.h"
#include "Input.h"
#include "Time.h"
#include "Colors.h"
#include "Random.h"
#include "Scene.h"

#include "Transform.h"
#include "Camera.h"
#include "Script.h"
#include "Sprite.h"
#include "ScriptList.h"
#include "TestScript.h"
#include "PlayerScript.h"
#include "ColliderList.h"
#include "BoxCollider.h"

int main(int argc, char** argv) {
	Window::Init("MiniNinja", { 512, 384 });
	Window::SetLetterboxColor(Colors::DARK_GREY);
	Textures::Init();
	Fonts::Init("Resources/Fonts/m5x7.ttf");
	Audio::Init();

	Scene scene = Scene();

	Entities::Index entityCamera = scene.AddEntity();
	scene.AddComponent<Transform>(entityCamera);
	scene.activeCamera = scene.AddComponent<Camera>(entityCamera);
	scene.activeCamera->SetDim({ 128, 96 });
	Window::SetRenderTarget(scene.activeCamera->GetTexture());

	Entities::Index playerEntity = scene.AddEntity();
	scene.AddComponent<Transform>(playerEntity);
	Sprite* playerSprite = scene.AddComponent<Sprite>(playerEntity);
	ScriptList* playerScriptList = scene.AddComponent<ScriptList>(playerEntity);
	PlayerScript* playerScript = Scripts::NewScript<PlayerScript>();
	playerScriptList->scripts.push_back(playerScript);
	ColliderList* playerColliderList = scene.AddComponent<ColliderList>(playerEntity);
	BoxCollider* playerCollider = Colliders::New<BoxCollider>();
	playerCollider->SetBox({ 0, 0, 16, 16 });
	playerCollider->doSolveCollision = true;
	playerColliderList->colliders.push_back(playerCollider);

	Entities::Index enemyEntitiy = scene.AddEntity();
	scene.AddComponent<Transform>(enemyEntitiy);
	Sprite* enemySprite = scene.AddComponent<Sprite>(enemyEntitiy);
	ScriptList* enemyScriptList = scene.AddComponent<ScriptList>(enemyEntitiy);
	TestScript* enemyScript = Scripts::NewScript<TestScript>();
	enemyScriptList->scripts.push_back(enemyScript);
	ColliderList* enemyColliderList = scene.AddComponent<ColliderList>(enemyEntitiy);
	BoxCollider* enemyCollider = Colliders::New<BoxCollider>();
	enemySprite->dim = { 32, 32 };
	enemyCollider->SetBox({ 0, 0, enemySprite->dim.x, enemySprite->dim.y });
	enemyColliderList->colliders.push_back(enemyCollider);

	while (true) {
		Window::HandleEvents();
		Input::HandleEvents();

		scene.Update();
		scene.DoCollisions();
		scene.LateUpdate();
		Window::SetDrawColor(Colors::LIGHT_GREY);
		Window::Paint();
		scene.Render();

		Window::PresentWindow();
		Time::LimitFramerate(60);
	}
	return 0;
}