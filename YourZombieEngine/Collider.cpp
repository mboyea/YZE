#include "Collider.h"
const char Collider::str[] = "Collider";
#include "ColliderSystem.h"

Collision Collider::DoNarrowCollision(Transform* transform, Collider* target, Transform* targetTransform) {
	return Collision(false);
}

bool Collider::IsBroadColliding(Transform* transform, Collider* target, Transform* targetTransform) {
	const Vector2i pos = transform->pos + aabb.GetPos();
	const Vector2i targetPos = targetTransform->pos + target->aabb.GetPos();
	return pos.x < targetPos.x + target->aabb.w
		&& pos.x + aabb.w > targetPos.x
		&& pos.y < targetPos.y + target->aabb.h
		&& pos.y + aabb.h > targetPos.y;
}

Collision Collider::DoCollision(Transform* transform, Collider* target, Transform* targetTransform) {
	if (IsBroadColliding(transform, target, targetTransform)) {
		return DoNarrowCollision(transform, target, targetTransform);
	}
	return Collision(false);
}

const Vector4i& Collider::GetAABB() const {
	return aabb;
}