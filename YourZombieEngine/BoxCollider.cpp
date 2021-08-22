#include "BoxCollider.h"
REGISTER_COLLIDER(BoxCollider);

Vector2i GetLineIntersection(const Vector2i& a, const Vector2i& b, const Vector2i& c, const Vector2i& d) {
	// line a-b represented as a1x + b1y = c1
	const int a1 = b.y - a.y;
	const int b1 = a.x - b.x;
	// line c-d represented as a2x + b2y = c2
	const int a2 = d.y - c.y;
	const int b2 = c.x - d.x;
	// check if lines are parallel
	const int determinant = (a1 * b2) - (a2 * b1);
	if (determinant == 0) {
		// lines are parallel
		return { INT_MAX, INT_MAX };
	}
	const int c1 = (a1 * a.x) + (b1 * a.y);
	const int c2 = (a2 * c.x) + (b2 * c.y);
	return { ((b2 * c1) - (b1 * c2)) / determinant, ((a1 * c2) - (a2 * c1)) / determinant };
}

int UnsafeGetLineIntersectionX(const Vector2i& a, const Vector2i& b, const Vector2i& c, const Vector2i& d) {
	// line a-b represented as a1x + b1y = c1
	const int a1 = b.y - a.y;
	const int b1 = a.x - b.x;
	const int c1 = (a1 * a.x) + (b1 * a.y);
	// line c-d represented as a2x + b2y = c2
	const int a2 = d.y - c.y;
	const int b2 = c.x - d.x;
	const int c2 = (a2 * c.x) + (b2 * c.y);
	return ((b2 * c1) - (b1 * c2)) / ((a1 * b2) - (a2 * b1));
}

// lhs is the movable collider, rhs won't move under any circumstances
Vector2i SolveSoloBoxCollision(const Collider* lhsCollider, Transform* lhsTransform, const Collider* rhsCollider, const Transform* rhsTransform) {
	Vector2i impulse = { 0, 0 };
	const Vector2i lhsLocalLeadingPoint = {
		lhsCollider->GetAABB().x + int(lhsTransform->pos.x - lhsTransform->lastPos.x > 0) * lhsCollider->GetAABB().w,
		lhsCollider->GetAABB().y + int(lhsTransform->pos.y - lhsTransform->lastPos.y > 0) * lhsCollider->GetAABB().h
	};
	const Vector2i lhsPathStart = lhsTransform->lastPos + lhsLocalLeadingPoint;
	const Vector2i lhsPathEnd = lhsTransform->pos + lhsLocalLeadingPoint;
	if (lhsPathStart.y < lhsPathEnd.y) {
		const Vector2i rhsTopLeft = rhsTransform->pos + rhsCollider->GetAABB().GetPos();
		const Vector2i rhsTopRight = {
			rhsTransform->pos.x + rhsCollider->GetAABB().x + rhsCollider->GetAABB().w,
			rhsTransform->pos.y + rhsCollider->GetAABB().y
		};
		if (lhsPathStart.x < lhsPathEnd.x) {
			if (UnsafeGetLineIntersectionX(lhsPathStart, lhsPathEnd, rhsTopLeft, rhsTopRight) >= rhsTopLeft.x) {
				// hit TOP
				impulse.y = rhsTopLeft.y - lhsPathEnd.y;
			} else {
				// hit LEFT
				impulse.x = rhsTopLeft.x - lhsPathEnd.x;
			}
		} else if (lhsPathStart.x > lhsPathEnd.x) {
			if (UnsafeGetLineIntersectionX(lhsPathStart, lhsPathEnd, rhsTopLeft, rhsTopRight) <= rhsTopRight.x) {
				// hit TOP
				impulse.y = rhsTopLeft.y - lhsPathEnd.y;
			} else {
				// hit RIGHT
				impulse.x = rhsTopRight.x - lhsPathEnd.x;
			}
		} else {
			// hit TOP
			impulse.y = rhsTopLeft.y - lhsPathEnd.y;
		}
	} else if (lhsPathStart.y > lhsPathEnd.y) {
		const Vector2i rhsBottomLeft = {
			rhsTransform->pos.x + rhsCollider->GetAABB().x,
			rhsTransform->pos.y + rhsCollider->GetAABB().y + rhsCollider->GetAABB().h
		};
		const Vector2i rhsBottomRight = {
			rhsTransform->pos.x + rhsCollider->GetAABB().x + rhsCollider->GetAABB().w,
			rhsTransform->pos.y + rhsCollider->GetAABB().y + rhsCollider->GetAABB().h
		};
		if (lhsPathStart.x < lhsPathEnd.x) {
			if (UnsafeGetLineIntersectionX(lhsPathStart, lhsPathEnd, rhsBottomLeft, rhsBottomRight) >= rhsBottomLeft.x) {
				// hit BOTTOM
				impulse.y = rhsBottomLeft.y - lhsPathEnd.y;
			} else {
				// hit LEFT
				impulse.x = rhsBottomLeft.x - lhsPathEnd.x;
			}
		}
		else if (lhsPathStart.x > lhsPathEnd.x) {
			if (UnsafeGetLineIntersectionX(lhsPathStart, lhsPathEnd, rhsBottomLeft, rhsBottomRight) <= rhsBottomRight.x) {
				// hit BOTTOM
				impulse.y = rhsBottomLeft.y - lhsPathEnd.y;
			} else {
				// hit RIGHT
				impulse.x = rhsBottomRight.x - lhsPathEnd.x;
			}
		} else {
			// hit BOTTOM
			impulse.y = rhsBottomLeft.y - lhsPathEnd.y;
		}
	} else {
		if (lhsPathStart.x < lhsPathEnd.x) {
			// hit LEFT
			impulse.x = rhsTransform->pos.x + rhsCollider->GetAABB().x - lhsPathEnd.x;
		} else if (lhsPathStart.x > lhsPathEnd.x) {
			// hit RIGHT
			impulse.x = rhsTransform->pos.x + rhsCollider->GetAABB().x + rhsCollider->GetAABB().w - lhsPathEnd.x;
		} else {
			// TODO: move this object based upon the movement of the other object
		}
	}
	lhsTransform->pos += impulse;
	return impulse;
}

std::pair<Vector2i, Vector2i> SolveDuoBoxCollision(const Collider* lhsCollider, Transform* lhsTransform, const Collider* rhsCollider, Transform* rhsTransform) {
	std::pair<Vector2i, Vector2i> impulse = { { 0, 0 }, { 0, 0 } };
	// TOOD: LOGIC
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