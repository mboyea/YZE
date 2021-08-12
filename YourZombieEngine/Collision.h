#ifndef COLLISION_H
#define COLLISION_H
#pragma once

#include "Collider.h"
#include "Entities.h"
#include "Vector2i.h"

class Collision {
public:
	Entities::Index callingEntity;
	Entities::Index collidingEntity;
	bool didCollide;
	Vector2i point;
};

#endif // !COLLISION_H