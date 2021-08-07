#include "Camera.h"
REGISTER_COMPONENT(Camera);
#include "Window.h"

Camera::Camera(Vector2i dim)
    : dim(dim), renderTexture(SDL_CreateTexture(Window::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, dim.x, dim.y)) {}

SDL_Texture* Camera::GetTexture() {
    return renderTexture;
}

Vector2i Camera::GetDim() {
    return dim;
}

void Camera::SetDim(Vector2i dim) {
}

Camera::operator SDL_Texture* () const {
    return renderTexture;
}