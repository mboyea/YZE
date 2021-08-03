#include "Window.h"
#include "Textures.h"
#include "Fonts.h"
#include "Audio.h"
#include "Input.h"
#include "Time.h"
#include "Colors.h"
#include "Random.h"

int main(int argc, char** argv) {
	Window::Init("MiniNinja", { 128, 96 });
	Window::SetLetterboxColor(Colors::DARK_GREY);
	Textures::Init();
	Fonts::Init("Resources/Fonts/m5x7.ttf");
	Audio::Init();
	Window::SetRenderTarget(Textures::GetDefault());
	while (true) {
		Window::HandleEvents();
		Input::HandleEvents();



		Window::PresentWindow();
		Time::LimitFramerate(60);
	}
	return 0;
}