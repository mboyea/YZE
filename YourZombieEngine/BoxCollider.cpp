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

Collision BoxCollider::DoNarrowCollision(Transform* transform, Collider* target, Transform* targetTransform) {
	{ // target is BoxCollider
		if (dynamic_cast<BoxCollider*>(target) != nullptr) {
			if (doSolveCollision) {
				const Vector2i offset = transform->pos - transform->lastPos;
				const Vector2i leadingPoint = {
					aabb.x + ((offset.x > 0) * aabb.w),
					aabb.y + ((offset.y > 0) * aabb.h)
				};
				const Vector2i pathStart = transform->lastPos + leadingPoint;
				const Vector2i pathEnd = transform->pos + leadingPoint;
				const Vector2i topLeft = targetTransform->pos + GetAABB().GetPos();
				const Vector2i topRight = {
					targetTransform->pos.x + GetAABB().GetPos().x + GetAABB().GetDim().x,
					targetTransform->pos.y + GetAABB().GetPos().y
				};
				const Vector2i bottomLeft = {
					targetTransform->pos.x + GetAABB().GetPos().x,
					targetTransform->pos.y + GetAABB().GetPos().y + GetAABB().GetDim().y
				};
				const Vector2i bottomRight = {
					targetTransform->pos.x + GetAABB().GetPos().x + GetAABB().GetDim().x,
					targetTransform->pos.y + GetAABB().GetPos().y + GetAABB().GetDim().y
				};
				std::pair<Vector2i, Vector2i> impulse;
				if (pathStart.y < pathEnd.y) {
					// can hit top, left, or right
					if (pathStart.x < pathEnd.x) {
						// can hit left or top
						if (GetLineIntersection(pathStart, pathEnd, topLeft, topRight).x >= topLeft.x) {
							// will hit top
							impulse.first = {
								0,
								targetTransform->pos.y - pathEnd.y
							};
							transform->pos = transform->pos + impulse.first;
							return Collision(true, impulse);
						}
						{
							// will hit left
							impulse.first = {
								targetTransform->pos.x - pathEnd.x,
								0
							};
							transform->pos = transform->pos + impulse.first;
							return Collision(true, impulse);
						}
					}
					if (pathStart.x > pathEnd.x) {
						// can hit right or top
						if (GetLineIntersection(pathStart, pathEnd, topLeft, topRight).x <= topRight.x) {
							// will hit top
							impulse.first = {
								0,
								targetTransform->pos.y - pathEnd.y
							};
							transform->pos = transform->pos + impulse.first;
							return Collision(true, impulse);
						}
						{
							// will hit right
							impulse.first = {
								targetTransform->pos.x - pathEnd.x + target->GetAABB().w,
								0
							};
							transform->pos = transform->pos + impulse.first;
							return Collision(true, impulse);
						}
					}
					{
						// will hit top
						impulse.first = {
							0,
							targetTransform->pos.y - pathEnd.y
						};
						transform->pos = transform->pos + impulse.first;
						return Collision(true, impulse);
					}
				}
				if (pathStart.y > pathEnd.y) {
					// can hit bottom, left, or right
					if (pathStart.x < pathEnd.x) {
						// can hit left or bottom
						if (GetLineIntersection(pathStart, pathEnd, bottomLeft, bottomRight).x >= bottomLeft.x) {
							// will hit bottom
							impulse.first = {
								0,
								targetTransform->pos.y - pathEnd.y + target->GetAABB().w
							};
							transform->pos = transform->pos + impulse.first;
							return Collision(true, impulse);
						}
						{
							// will hit left
							impulse.first = {
								targetTransform->pos.x - pathEnd.x,
								0
							};
							transform->pos = transform->pos + impulse.first;
							return Collision(true, impulse);
						}
					}
					if (pathStart.x > pathEnd.x) {
						// can hit right or bottom
						if (GetLineIntersection(pathStart, pathEnd, bottomLeft, bottomRight).x <= bottomRight.x) {
							// will hit bottom
							impulse.first = {
								0,
								targetTransform->pos.y - pathEnd.y + target->GetAABB().w
							};
							transform->pos = transform->pos + impulse.first;
							return Collision(true, impulse);
						}
						{
							// will hit right
							impulse.first = {
								targetTransform->pos.x - pathEnd.x + target->GetAABB().w,
								0
							};
							transform->pos = transform->pos + impulse.first;
							return Collision(true, impulse);
						}
					}
					{
						// will hit bottom
						impulse.first = {
							0,
							targetTransform->pos.y - pathEnd.y + target->GetAABB().w
						};
						transform->pos = transform->pos + impulse.first;
						return Collision(true, impulse);
					}
				}
				{
					// can hit left or right
					if (pathStart.x < pathEnd.x) {
						// will hit left
						impulse.first = {
							targetTransform->pos.x - pathEnd.x,
							0
						};
						transform->pos = transform->pos + impulse.first;
						return Collision(true, impulse);
					}
					{
						// will hit right
						impulse.first = {
							targetTransform->pos.x - pathEnd.x + target->GetAABB().w,
							0
						};
						transform->pos = transform->pos + impulse.first;
						return Collision(true, impulse);
					}
				}
				
				//	impulse.first = {
				//		int(/*is horizontal*/) * (targetTransform->pos.x - pathEnd.x + int(/*is right*/offset.x < 0) * (target->GetAABB().w)),
				//		int(/*is vertical*/) * (targetTransform->pos.y - pathEnd.y + int(/*is bottom*/offset.y < 0) * (target->GetAABB().h))
				//	};
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