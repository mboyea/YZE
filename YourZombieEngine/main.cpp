#include "Window.h"
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
	Window::Init("MiniNinja", { 128, 96 });
	Window::SetLetterboxColor(Colors::DARK_GREY);
	Textures::Init();
	Fonts::Init("Resources/Fonts/m5x7.ttf");
	Audio::Init();

	Scene scene = Scene();
	Entities::Index camera = scene.AddEntity();
	scene.AddComponent<Transform>(camera);
	scene.AddComponent<Camera>(camera);
	scene.cameraEntity = camera;
	Window::SetRenderTarget((SDL_Texture*)scene.GetComponent<Camera>(scene.cameraEntity));
	Entities::Index sprite = scene.AddEntity();
	scene.AddComponent<Transform>(sprite);
	scene.AddComponent<Sprite>(sprite);
	while (true) {
		Window::HandleEvents();
		Input::HandleEvents();



		Window::PresentWindow();
		Time::LimitFramerate(60);
	}
	return 0;
}