#ifndef BOX_COLLIDER_H
#define BOX_COLLIDER_H
#pragma once

#include "Collider.h"

class BoxCollider : public Collider {
	INIT_COLLIDER;
protected:
	virtual Collision DoNarrowCollision(Transform* transform, Collider* target, Transform* targetTransform);
public:
	void SetBox(Vector4i rect);
	void SetPos(Vector2i pos);
	void SetDim(Vector2i dim);
	Vector2i GetPos();
	Vector2i GetDim();
};

#endif // !BOX_COLLIDER_H