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
	virtual std::ostream& Serialize(std::ostream& os) override;
	virtual std::istream& Deserialize(std::istream& is) override;
};

#endif // !BOX_COLLIDER_H