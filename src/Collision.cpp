#include "Collision.h"

Collision::Collision(bool didCollide, std::pair<Vector2i, Vector2i> lmpulse)
	: didCollide(didCollide), impulse(impulse) {}

Collision::Collision(bool didCollide, Vector2i lhsImpulse, Vector2i rhsImpulse)
	: didCollide(didCollide), impulse({ lhsImpulse, rhsImpulse }) {}