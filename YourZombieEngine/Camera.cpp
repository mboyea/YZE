#include "Camera.h"
REGISTER_COMPONENT(Camera);
#include "Window.h"
#include "Textures.h"

Camera::Camera(Vector2i dim)
    : renderTexture(SDL_CreateTexture(Window::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, dim.x, dim.y)) {}

SDL_Texture* Camera::GetTexture() {
    return renderTexture;
}

Vector2i Camera::GetDim() {
    return Textures::GetDim(renderTexture);
}

void Camera::SetDim(Vector2i dim) {
    SDL_DestroyTexture(renderTexture);
    renderTexture = SDL_CreateTexture(Window::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, dim.x, dim.y);
}

Camera::operator SDL_Texture* () const {
    return renderTexture;
}

std::ostream& Camera::Serialize(std::ostream& os) {
    // TODO: logic
    return os;
}

std::istream& Camera::Deserialize(std::istream& is) {
    // TODO: logic
    return is;
}
