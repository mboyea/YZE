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
	return { ((b2 * c1) - (b1 * c2)) / determinant,((a1 * c2) - (a2 * c1)) / determinant };
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

Collision BoxCollider::DoNarrowCollision(Transform* transform, Collider* target, Transform* targetTransform) {
	{ // target is BoxCollider
		if (dynamic_cast<BoxCollider*>(target) != nullptr) {
			if (doSolveCollision) {
				//	impulse.first = {
				//		int(/*is horizontal*/) * (targetTransform->pos.x - pathEnd.x + int(/*is right*/offset.x < 0) * (target->GetAABB().w)),
				//		int(/*is vertical*/) * (targetTransform->pos.y - pathEnd.y + int(/*is bottom*/offset.y < 0) * (target->GetAABB().h))
				//	};
				std::pair<Vector2i, Vector2i> impulse = { { 0, 0 }, { 0, 0 } };
				// calculate the position of lhs
				const Vector2i lhsStartPos = transform->lastPos + aabb.GetPos();
				const Vector2i lhsEndPos = transform->pos + aabb.GetPos();
				// calculate the position of rhs
				const Vector2i rhsStartPos = targetTransform->lastPos + target->GetAABB().GetPos();
				const Vector2i rhsEndPos = targetTransform->pos + target->GetAABB().GetPos();
				// calculate the leading point of the lhs rectangle in local space
				const Vector2i lhsLocalLeadingPoint = {
					aabb.x + int(transform->pos.x - transform->lastPos.x > 0) * (aabb.w),
					aabb.y + int(transform->pos.y - transform->lastPos.y > 0) * (aabb.h)
				};
				// calculate the [leading point of the lhs rectangle]'s start & end points
				const Vector2i lhsPathStart = lhsStartPos + lhsLocalLeadingPoint;
				const Vector2i lhsPathEnd = lhsEndPos + lhsLocalLeadingPoint;
				// determine the impulse required to resolve the collision based upon the path of the leading point
				if (lhsPathStart.y < lhsPathEnd.y) {
					// hit TOP, LEFT, or RIGHT
					const Vector2i topLeft = rhsEndPos;
					const Vector2i topRight = {
						rhsEndPos.x + target->GetAABB().GetDim().x,
						rhsEndPos.y
					};
					if (lhsPathStart.x < lhsPathEnd.x) {
						// hit TOP or LEFT
						if (UnsafeGetLineIntersectionX(lhsPathStart, lhsPathEnd, topLeft, topRight) >= topLeft.x) {
							// hit TOP
							impulse.first.y = rhsEndPos.y + aabb.GetPos().y - lhsPathEnd.y;
						} else {
							// hit LEFT
							impulse.first.x = rhsEndPos.x + aabb.GetPos().x - lhsPathEnd.x;
						}
					} else if (lhsPathStart.x > lhsPathEnd.x) {
						// hit TOP or RIGHT
						if (UnsafeGetLineIntersectionX(lhsPathStart, lhsPathEnd, topLeft, topRight) <= topRight.x) {
							// hit TOP
							impulse.first.y = rhsEndPos.y + aabb.GetPos().y - lhsPathEnd.y;
						} else {
							// hit RIGHT
							impulse.first.x = rhsEndPos.x + aabb.GetPos().x - lhsPathEnd.x + target->GetAABB().w;
						}
					} else {
						// hit TOP
						impulse.first.y = rhsEndPos.y + aabb.GetPos().y - lhsPathEnd.y;
					}
				} else if (lhsPathStart.y > lhsPathEnd.y) {
					// hit BOTTOM, LEFT, or RIGHT
					const Vector2i bottomLeft = {
						rhsEndPos.x,
						rhsEndPos.y + target->GetAABB().GetDim().y
					};
					const Vector2i bottomRight = {
						rhsEndPos.x + target->GetAABB().GetDim().x,
						rhsEndPos.y + target->GetAABB().GetDim().y
					};
					if (lhsPathStart.x < lhsPathEnd.x) {
						// hit BOTTOM or LEFT
						if (UnsafeGetLineIntersectionX(lhsPathStart, lhsPathEnd, bottomLeft, bottomRight) >= bottomLeft.x) {
							// hit BOTTOM
							impulse.first.y = rhsEndPos.y + aabb.GetPos().y - lhsPathEnd.y + target->GetAABB().h;
						} else {
							// hit LEFT
							impulse.first.x = rhsEndPos.x + aabb.GetPos().x - lhsPathEnd.x;
						}
					} else if (lhsPathStart.x > lhsPathEnd.x) {
						// hit BOTTOM or RIGHT
						if (UnsafeGetLineIntersectionX(lhsPathStart, lhsPathEnd, bottomLeft, bottomRight) <= bottomRight.x) {
							// hit BOTTOM
							impulse.first.y = rhsEndPos.y + aabb.GetPos().y - lhsPathEnd.y + target->GetAABB().h;
						} else {
							// hit RIGHT
							impulse.first.x = rhsEndPos.x + aabb.GetPos().x - lhsPathEnd.x + target->GetAABB().w;
						}
					} else {
						// hit BOTTOM
						impulse.first.y = rhsEndPos.y + aabb.GetPos().y - lhsPathEnd.y + target->GetAABB().h;
					}
				} else {
					// hit LEFT or RIGHT
					if (lhsPathStart.x < lhsPathEnd.x) {
						// hit LEFT
						impulse.first.x = rhsEndPos.x + aabb.GetPos().x - lhsPathEnd.x;
					}
					else if (lhsPathStart.x > lhsPathEnd.x) {
						// hit RIGHT
						impulse.first.x = rhsEndPos.x + aabb.GetPos().x - lhsPathEnd.x + target->GetAABB().w;
					} else {
						// TODO: move this object based upon the movement of the other object
					}
				}
				transform->pos = transform->pos + impulse.first;
				return Collision(true, impulse);
			}
			return Collision(true);
		}
	}
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