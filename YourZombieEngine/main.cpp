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

	Entities::Index entitySprite = scene.AddEntity();
	scene.AddComponent<Transform>(entitySprite);
	Sprite* sprite = scene.AddComponent<Sprite>(entitySprite);

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