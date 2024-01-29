#ifndef COLLISION_H
#define COLLISION_H
#pragma once

#include "Entities.h"
#include "Vector2i.h"
#include <vector>

struct Collision {
	Collision(bool didCollide = false, std::pair<Vector2i, Vector2i> lmpulse = { { 0, 0 }, { 0, 0 } });
	Collision(bool didCollide, Vector2i lhsImpulse, Vector2i rhsImpulse);
	bool didCollide;
	// the amount which each collider was moved to resolve collision
	std::pair<Vector2i, Vector2i> impulse;
};

#endif // !COLLISION_H