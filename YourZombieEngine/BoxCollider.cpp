#include "BoxCollider.h"
REGISTER_COLLIDER(BoxCollider);

// lhs is a movable collider, rhs won't move under any circumstances
Vector2i SolveSoloBoxCollision(const Collider* lhsCollider, Transform* lhsTransform, const Collider* rhsCollider, const Transform* rhsTransform) {
	Vector2i impulse = { 0, 0 };

	const bool lhsIsRightCollision = rhsTransform->lastPos.x + rhsCollider->GetAABB().x > lhsTransform->lastPos.x + lhsCollider->GetAABB().x;
	const bool lhsIsBottomCollision = rhsTransform->lastPos.y + rhsCollider->GetAABB().y > lhsTransform->lastPos.y + lhsCollider->GetAABB().y;
	const Vector2i lhsCollisionPoint = {
		lhsTransform->lastPos.x + lhsCollider->GetAABB().x + int(lhsIsRightCollision) * lhsCollider->GetAABB().w,
		lhsTransform->lastPos.y + lhsCollider->GetAABB().y + int(lhsIsBottomCollision) * lhsCollider->GetAABB().h
	};
	const Vector2i rhsCollisionLine = {
		rhsTransform->lastPos.x + rhsCollider->GetAABB().x + int(!lhsIsRightCollision) * rhsCollider->GetAABB().w,
		rhsTransform->lastPos.y + rhsCollider->GetAABB().y + int(!lhsIsBottomCollision) * rhsCollider->GetAABB().h
	};
	const Vector2i lhsMomentum = lhsTransform->pos - lhsTransform->lastPos;
	const Vector2i rhsMomentum = rhsTransform->pos - rhsTransform->lastPos;
	float ty;
	float tx;
	if (rhsMomentum.y == lhsMomentum.y) ty = -1.f;
	else if (rhsCollisionLine.y == lhsCollisionPoint.y) ty = 0.f;
	else ty = (float)(rhsCollisionLine.y - lhsCollisionPoint.y) / (float)(lhsMomentum.y - rhsMomentum.y);
	if (rhsMomentum.x == lhsMomentum.x) tx = -1.f;
	else if (rhsCollisionLine.x == lhsCollisionPoint.x) tx = 0.f;
	else tx = (float)(rhsCollisionLine.x - lhsCollisionPoint.x) / (float)(lhsMomentum.x - rhsMomentum.x);
	if ((ty >= tx && ty <= 1.f) || tx >= 1.f) { // rhs hits lhs on top/or/bottom side
		if (lhsIsBottomCollision)
			impulse.y = rhsTransform->pos.y + rhsCollider->GetAABB().y - (lhsTransform->pos.y + lhsCollider->GetAABB().y + lhsCollider->GetAABB().h);
		else impulse.y = rhsTransform->pos.y + rhsCollider->GetAABB().y + rhsCollider->GetAABB().h - (lhsTransform->pos.y + lhsCollider->GetAABB().y);
	} else { // rhs hits lhs on left/or/right side
		if (lhsIsRightCollision)
			impulse.x = rhsTransform->pos.x + rhsCollider->GetAABB().x - (lhsTransform->pos.x + lhsCollider->GetAABB().x + lhsCollider->GetAABB().w);
		else impulse.x = rhsTransform->pos.x + rhsCollider->GetAABB().x + rhsCollider->GetAABB().w - (lhsTransform->pos.x + lhsCollider->GetAABB().x);
	}

	lhsTransform->pos += impulse;
	return impulse;
}

// both lhs and rhs are movable colliders and their movement is solved such that the momentum of one object can push another.
std::pair<Vector2i, Vector2i> SolveDuoBoxCollision(const Collider* lhsCollider, Transform* lhsTransform, const Collider* rhsCollider, Transform* rhsTransform) {
	std::pair<Vector2i, Vector2i> impulse = { { 0, 0 }, { 0, 0 } };

	const bool lhsIsRightCollision = rhsTransform->lastPos.x + rhsCollider->GetAABB().x > lhsTransform->lastPos.x + lhsCollider->GetAABB().x;
	const bool lhsIsBottomCollision = rhsTransform->lastPos.y + rhsCollider->GetAABB().y > lhsTransform->lastPos.y + lhsCollider->GetAABB().y;
	const Vector2i lhsCollisionPoint = {
		lhsTransform->lastPos.x + lhsCollider->GetAABB().x + int(lhsIsRightCollision) * lhsCollider->GetAABB().w,
		lhsTransform->lastPos.y + lhsCollider->GetAABB().y + int(lhsIsBottomCollision) * lhsCollider->GetAABB().h
	};
	const Vector2i rhsCollisionLine = {
		rhsTransform->lastPos.x + rhsCollider->GetAABB().x + int(!lhsIsRightCollision) * rhsCollider->GetAABB().w,
		rhsTransform->lastPos.y + rhsCollider->GetAABB().y + int(!lhsIsBottomCollision) * rhsCollider->GetAABB().h
	};
	const Vector2i lhsPathEnd = {
		lhsTransform->pos.x + lhsCollider->GetAABB().x + int(lhsIsRightCollision) * lhsCollider->GetAABB().w,
		lhsTransform->pos.y + lhsCollider->GetAABB().y + int(lhsIsBottomCollision) * lhsCollider->GetAABB().h
	};
	const Vector2i rhsPathEnd = {
		rhsTransform->pos.x + rhsCollider->GetAABB().x + int(!lhsIsRightCollision) * rhsCollider->GetAABB().w,
		rhsTransform->pos.y + rhsCollider->GetAABB().y + int(!lhsIsBottomCollision) * rhsCollider->GetAABB().h
	};
	const Vector2i lhsMomentum = lhsTransform->pos - lhsTransform->lastPos;
	const Vector2i rhsMomentum = rhsTransform->pos - rhsTransform->lastPos;
	float ty;
	float tx;
	if (rhsMomentum.y == lhsMomentum.y) ty = -1.f;
	else if (rhsCollisionLine.y == lhsCollisionPoint.y) ty = 0.f;
	else ty = (float)(rhsCollisionLine.y - lhsCollisionPoint.y) / (float)(lhsMomentum.y - rhsMomentum.y);
	if (rhsMomentum.x == lhsMomentum.x) tx = -1.f;
	else if (rhsCollisionLine.x == lhsCollisionPoint.x) tx = 0.f;
	else tx = (float)(rhsCollisionLine.x - lhsCollisionPoint.x) / (float)(lhsMomentum.x - rhsMomentum.x);
	const Vector2i lhsCollisionMomentum = {
		(int)std::round((1.f - tx) * lhsMomentum.x),
		(int)std::round((1.f - ty) * lhsMomentum.y)
	};
	const Vector2i rhsCollisionMomentum = {
		lhsCollisionMomentum.x + rhsPathEnd.x - lhsPathEnd.x,
		lhsCollisionMomentum.y + rhsPathEnd.y - lhsPathEnd.y
	};
	const Vector2i totalCollisionMomentum = {
		lhsCollisionMomentum.x + rhsCollisionMomentum.x,
		lhsCollisionMomentum.y + rhsCollisionMomentum.y
	};
	if ((ty >= tx && ty <= 1.f) || tx >= 1.f) { // rhs hits lhs on top/or/bottom side
		impulse.first.y = -lhsCollisionMomentum.y + totalCollisionMomentum.y;
		impulse.second.y = -rhsCollisionMomentum.y + totalCollisionMomentum.y;
	} else { // rhs hits lhs on left/or/right side
		impulse.first.x = -lhsCollisionMomentum.x + totalCollisionMomentum.x;
		impulse.second.x = -rhsCollisionMomentum.x + totalCollisionMomentum.x;
	}

	lhsTransform->pos += impulse.first;
	rhsTransform->pos += impulse.second;
	return impulse;
}

Collision BoxCollider::DoNarrowCollision(Transform* transform, Collider* target, Transform* targetTransform) {
	// target = BoxCollider
	if (dynamic_cast<BoxCollider*>(target) != nullptr) {
		if (doSolveCollision) {
			if (target->doSolveCollision) {
				return Collision(true, SolveDuoBoxCollision(this, transform, target, targetTransform));
			}
			return Collision(true, { SolveSoloBoxCollision(this, transform, target, targetTransform), { 0, 0 } });
		}
		if (target->doSolveCollision) {
			return Collision(true, { { 0, 0 }, SolveSoloBoxCollision(target, targetTransform, this, transform) });
		}
		return Collision(true);
	}
	// target = ???
	return Collision(false);
}

void BoxCollider::SetBox(Vector4i rect) {
	aabb = rect;
}

void BoxCollider::SetPos(Vector2i pos) {
	aabb.x = pos.x;
	aabb.y = pos.y;
}

void BoxCollider::SetDim(Vector2i dim) {
	aabb.w = dim.x;
	aabb.h = dim.y;
}

Vector2i BoxCollider::GetPos() {
	return aabb.GetPos();
}

Vector2i BoxCollider::GetDim() {
	return aabb.GetDim();
}

std::ostream& BoxCollider::Serialize(std::ostream& os) {
	return Collider::Serialize(os) << ' ' << aabb;
}

std::istream& BoxCollider::Deserialize(std::istream& is) {
	return Collider::Deserialize(is) >> aabb;
}