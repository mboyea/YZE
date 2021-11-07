#include "BoxCollider.h"
REGISTER_COLLIDER(BoxCollider);
#include "Vector2f.h"
#include <math.h>

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

	const bool lhsIsRightLeading = lhsTransform->lastPos.x < lhsTransform->pos.x;
	const bool lhsIsBottomLeading = lhsTransform->lastPos.y < lhsTransform->pos.y;
	const Vector2i lhsLocalLeadingPoint = {
		lhsCollider->GetAABB().x + int(lhsIsRightLeading) * lhsCollider->GetAABB().w,
		lhsCollider->GetAABB().y + int(lhsIsBottomLeading) * lhsCollider->GetAABB().h
	};
	const Vector2i lhsPathStart = lhsTransform->lastPos + lhsLocalLeadingPoint;
	const Vector2i lhsPathEnd = lhsTransform->pos + lhsLocalLeadingPoint;
	const Vector2i lhsMomentum = lhsTransform->pos - lhsTransform->lastPos;

	const bool rhsIsRightLeading = rhsTransform->lastPos.x < rhsTransform->pos.x;
	const bool rhsIsBottomLeading = rhsTransform->lastPos.y < rhsTransform->pos.y;
	const Vector2i rhsLocalLeadingPoint = {
		rhsCollider->GetAABB().x + int(rhsIsRightLeading) * rhsCollider->GetAABB().w,
		rhsCollider->GetAABB().y + int(rhsIsBottomLeading) * rhsCollider->GetAABB().h
	};
	const Vector2i rhsPathStart = rhsTransform->lastPos + rhsLocalLeadingPoint;
	const Vector2i rhsPathEnd = rhsTransform->pos + rhsLocalLeadingPoint;
	const Vector2i rhsMomentum = rhsTransform->pos - rhsTransform->lastPos;

	if (lhsMomentum.x == 0 && lhsMomentum.y == 0) { // RHS PUSHES LHS COMPLETELY
		if (rhsTransform->lastPos.y < rhsTransform->pos.y) { // rhs didn't hit the bottom of lhs
			const Vector2i lhsTopLeft = lhsTransform->pos + lhsCollider->GetAABB().GetPos();
			const Vector2i lhsTopRight = {
				lhsTransform->pos.x + lhsCollider->GetAABB().x + lhsCollider->GetAABB().w,
				lhsTransform->pos.y + lhsCollider->GetAABB().y
			};
			if (rhsTransform->lastPos.x < rhsTransform->pos.x) { // rhs didn't hit the right of lhs
				if (UnsafeGetLineIntersectionX(rhsPathStart, rhsPathEnd, lhsTopLeft, lhsTopRight) >= lhsTopLeft.x) {
					// hit TOP
					impulse.y = rhsPathEnd.y - lhsTopLeft.y;
				} else {
					// hit LEFT
					impulse.x = rhsPathEnd.x - lhsTopLeft.x;
				}
			} else if (rhsTransform->lastPos.x > rhsTransform->pos.x) { // rhs didn't hit the left of lhs
				if (UnsafeGetLineIntersectionX(rhsPathStart, rhsPathEnd, lhsTopLeft, lhsTopRight) <= lhsTopRight.x) {
					// hit TOP
					impulse.y = rhsPathEnd.y - lhsTopLeft.y;
				} else {
					// hit RIGHT
					impulse.x = rhsPathEnd.x - lhsTopRight.x;
				}
			} else { // rhs didn't hit the side of lhs
				// hit TOP
				impulse.y = rhsPathEnd.y - lhsTopLeft.y;
			}
		} else if (rhsTransform->lastPos.y > rhsTransform->pos.y) { // rhs didn't hit the top of lhs
			const Vector2i lhsBottomLeft = {
				lhsTransform->pos.x + lhsCollider->GetAABB().x,
				lhsTransform->pos.y + lhsCollider->GetAABB().y + lhsCollider->GetAABB().h
			};
			const Vector2i lhsBottomRight = {
				lhsTransform->pos.x + lhsCollider->GetAABB().x + lhsCollider->GetAABB().w,
				lhsTransform->pos.y + lhsCollider->GetAABB().y + lhsCollider->GetAABB().h
			};
			if (rhsTransform->lastPos.x < rhsTransform->pos.x) { // rhs didn't hit the right of lhs
				if (UnsafeGetLineIntersectionX(rhsPathStart, rhsPathEnd, lhsBottomLeft, lhsBottomRight) >= lhsBottomLeft.x) {
					// hit BOTTOM
					impulse.y = rhsPathEnd.y - lhsBottomLeft.y;
				} else {
					// hit LEFT
					impulse.x = rhsPathEnd.x - lhsBottomLeft.x;
				}
			} else if (rhsTransform->lastPos.x > rhsTransform->pos.x) { // rhs didn't hit the left of lhs
				if (UnsafeGetLineIntersectionX(rhsPathStart, rhsPathEnd, lhsBottomLeft, lhsBottomRight) <= lhsBottomRight.x) {
					// hit BOTTOM
					impulse.y = rhsPathEnd.y - lhsBottomLeft.y;
				} else {
					// hit RIGHT
					impulse.x = rhsPathEnd.x - lhsBottomRight.x;
				}
			} else { // rhs didn't hit the side of lhs
				// hit BOTTOM
				impulse.y = rhsPathEnd.y - lhsBottomLeft.y;
			}
		} else { // rhs hit the side of lhs
			if (rhsTransform->lastPos.x < rhsTransform->pos.x) { // rhs didn't hit the right of lhs
				// hit LEFT
				impulse.x = rhsPathEnd.x - lhsTransform->pos.x - lhsCollider->GetAABB().x;
			} else if (rhsTransform->lastPos.x > rhsTransform->pos.x) { // rhs didn't hit the left of lhs
				// hit RIGHT
				impulse.x = rhsPathEnd.x - lhsTransform->pos.x - lhsCollider->GetAABB().x - lhsCollider->GetAABB().w;
			} else { // rhs actually didn't move
			}
		}
	} else if (rhsMomentum.x == 0 && rhsMomentum.y == 0) { // RHS STOPS LHS COMPLETELY
		if (lhsTransform->lastPos.y < lhsTransform->pos.y) { // lhs didn't hit the bottom of rhs
			const Vector2i rhsTopLeft = rhsTransform->pos + rhsCollider->GetAABB().GetPos();
			const Vector2i rhsTopRight = {
				rhsTransform->pos.x + rhsCollider->GetAABB().x + rhsCollider->GetAABB().w,
				rhsTransform->pos.y + rhsCollider->GetAABB().y
			};
			if (lhsTransform->lastPos.x < lhsTransform->pos.x) { // lhs didn't hit the right of rhs
				if (UnsafeGetLineIntersectionX(lhsPathStart, lhsPathEnd, rhsTopLeft, rhsTopRight) >= rhsTopLeft.x) {
					// hit TOP
					impulse.y = rhsTopLeft.y - lhsPathEnd.y;
				} else {
					// hit LEFT
					impulse.x = rhsTopLeft.x - lhsPathEnd.x;
				}
			} else if (lhsTransform->lastPos.x > lhsTransform->pos.x) { // lhs didn't hit the left of rhs
				if (UnsafeGetLineIntersectionX(lhsPathStart, lhsPathEnd, rhsTopLeft, rhsTopRight) <= rhsTopRight.x) {
					// hit TOP
					impulse.y = rhsTopLeft.y - lhsPathEnd.y;
				} else {
					// hit RIGHT
					impulse.x = rhsTopRight.x - lhsPathEnd.x;
				}
			} else { // lhs didn't hit the side of rhs
				// hit TOP
				impulse.y = rhsTopLeft.y - lhsPathEnd.y;
			}
		} else if (lhsTransform->lastPos.y > lhsTransform->pos.y) { // lhs didn't hit the top of rhs
			const Vector2i rhsBottomLeft = {
				rhsTransform->pos.x + rhsCollider->GetAABB().x,
				rhsTransform->pos.y + rhsCollider->GetAABB().y + rhsCollider->GetAABB().h
			};
			const Vector2i rhsBottomRight = {
				rhsTransform->pos.x + rhsCollider->GetAABB().x + rhsCollider->GetAABB().w,
				rhsTransform->pos.y + rhsCollider->GetAABB().y + rhsCollider->GetAABB().h
			};
			if (lhsTransform->lastPos.x < lhsTransform->pos.x) { // lhs didn't hit the right of rhs
				if (UnsafeGetLineIntersectionX(lhsPathStart, lhsPathEnd, rhsBottomLeft, rhsBottomRight) >= rhsBottomLeft.x) {
					// hit BOTTOM
					impulse.y = rhsBottomLeft.y - lhsPathEnd.y;
				} else {
					// hit LEFT
					impulse.x = rhsBottomLeft.x - lhsPathEnd.x;
				}
			} else if (lhsTransform->lastPos.x > lhsTransform->pos.x) { // lhs didn't hit the left of rhs
				if (UnsafeGetLineIntersectionX(lhsPathStart, lhsPathEnd, rhsBottomLeft, rhsBottomRight) <= rhsBottomRight.x) {
					// hit BOTTOM
					impulse.y = rhsBottomLeft.y - lhsPathEnd.y;
				} else {
					// hit RIGHT
					impulse.x = rhsBottomRight.x - lhsPathEnd.x;
				}
			} else { // lhs didn't hit the side of rhs
				// hit BOTTOM
				impulse.y = rhsBottomLeft.y - lhsPathEnd.y;
			}
		} else { // lhs hit the side of rhs
			if (lhsTransform->lastPos.x < lhsTransform->pos.x) { // lhs didn't hit the right of rhs
				// hit LEFT
				impulse.x = rhsTransform->pos.x + rhsCollider->GetAABB().x - lhsPathEnd.x;
			} else if (lhsTransform->lastPos.x > lhsTransform->pos.x) { // lhs didn't hit the left of rhs
				// hit RIGHT
				impulse.x = rhsTransform->pos.x + rhsCollider->GetAABB().x + rhsCollider->GetAABB().w - lhsPathEnd.x;
			} else { // lhs actually didn't move
			}
		}
	} else { // RHS PUSHES LHS IN THE COLLISION AXIS WHILE LHS RETAINS ITS OTHER AXIS OF MOMENTUM// calculate at what % of distance they collide
		Vector2f percentToCollide = {
			(float)(lhsPathStart.x - rhsPathStart.x) / (float)(rhsMomentum.x - lhsMomentum.x),
			(float)(lhsPathStart.y - rhsPathStart.y) / (float)(rhsMomentum.y - lhsMomentum.y)
		};
		if (rhsMomentum.x - lhsMomentum.x == 0 || rhsTransform->lastPos.x == rhsTransform->pos.x) {
			percentToCollide.x = -1;
		}
		if (rhsMomentum.y - lhsMomentum.y == 0 || rhsTransform->lastPos.y == rhsTransform->pos.y) {
			percentToCollide.y = -1;
		}
		if (percentToCollide.y > percentToCollide.x) { // rhs hits lhs vertically
			if (1) { // rhs hits lhs on the bottom
				impulse.y = rhsTransform->pos.y + rhsCollider->GetAABB().y - (lhsTransform->pos.y + lhsCollider->GetAABB().y + lhsCollider->GetAABB().h);
			} else { // rhs hits lhs on the top
				impulse.y = rhsTransform->pos.y + rhsCollider->GetAABB().y + rhsCollider->GetAABB().w - (lhsTransform->pos.y + lhsCollider->GetAABB().y);
			}
		} else { // rhs hits lhs horizontally
			if (1) { // rhs hits lhs on the right
				impulse.x = rhsTransform->pos.x + rhsCollider->GetAABB().x - (lhsTransform->pos.x + lhsCollider->GetAABB().x + lhsCollider->GetAABB().w);
			} else { // rhs hits lhs on the left
				impulse.x = rhsTransform->pos.x + rhsCollider->GetAABB().x + rhsCollider->GetAABB().h - (lhsTransform->pos.x + lhsCollider->GetAABB().x);
			}
		}
	}
	lhsTransform->pos += impulse;
	return impulse;
}

std::pair<Vector2i, Vector2i> SolveDuoBoxCollision(const Collider* lhsCollider, Transform* lhsTransform, const Collider* rhsCollider, Transform* rhsTransform) {
	std::pair<Vector2i, Vector2i> impulse = { { 0, 0 }, { 0, 0 } };

	const bool lhsIsRightLeading = lhsTransform->lastPos.x < lhsTransform->pos.x;
	const bool lhsIsBottomLeading = lhsTransform->lastPos.y < lhsTransform->pos.y;
	const Vector2i lhsLocalLeadingPoint = {
		lhsCollider->GetAABB().x + int(lhsIsRightLeading) * lhsCollider->GetAABB().w,
		lhsCollider->GetAABB().y + int(lhsIsBottomLeading) * lhsCollider->GetAABB().h
	};
	const Vector2i lhsPathStart = lhsTransform->lastPos + lhsLocalLeadingPoint;
	const Vector2i lhsPathEnd = lhsTransform->pos + lhsLocalLeadingPoint;
	const Vector2i lhsMomentum = lhsTransform->pos - lhsTransform->lastPos;

	const bool rhsIsRightLeading = rhsTransform->lastPos.x < rhsTransform->pos.x;
	const bool rhsIsBottomLeading = rhsTransform->lastPos.y < rhsTransform->pos.y;
	const Vector2i rhsLocalLeadingPoint = {
		rhsCollider->GetAABB().x + int(rhsIsRightLeading) * rhsCollider->GetAABB().w,
		rhsCollider->GetAABB().y + int(rhsIsBottomLeading) * rhsCollider->GetAABB().h
	};
	const Vector2i rhsPathStart = rhsTransform->lastPos + rhsLocalLeadingPoint;
	const Vector2i rhsPathEnd = rhsTransform->pos + rhsLocalLeadingPoint;
	const Vector2i rhsMomentum = rhsTransform->pos - rhsTransform->lastPos;

	if (lhsMomentum.x == 0 && lhsMomentum.y == 0) { // RHS PUSHES LHS COMPLETELY
		if (rhsTransform->lastPos.y < rhsTransform->pos.y) { // rhs didn't hit the bottom of lhs
			const Vector2i lhsTopLeft = lhsTransform->pos + lhsCollider->GetAABB().GetPos();
			const Vector2i lhsTopRight = {
				lhsTransform->pos.x + lhsCollider->GetAABB().x + lhsCollider->GetAABB().w,
				lhsTransform->pos.y + lhsCollider->GetAABB().y
			};
			if (rhsTransform->lastPos.x < rhsTransform->pos.x) { // rhs didn't hit the right of lhs
				if (UnsafeGetLineIntersectionX(rhsPathStart, rhsPathEnd, lhsTopLeft, lhsTopRight) >= lhsTopLeft.x) {
					// hit TOP
					impulse.first.y = rhsPathEnd.y - lhsTopLeft.y;
				} else {
					// hit LEFT
					impulse.first.x = rhsPathEnd.x - lhsTopLeft.x;
				}
			} else if (rhsTransform->lastPos.x > rhsTransform->pos.x) { // rhs didn't hit the left of lhs
				if (UnsafeGetLineIntersectionX(rhsPathStart, rhsPathEnd, lhsTopLeft, lhsTopRight) <= lhsTopRight.x) {
					// hit TOP
					impulse.first.y = rhsPathEnd.y - lhsTopLeft.y;
				} else {
					// hit RIGHT
					impulse.first.x = rhsPathEnd.x - lhsTopRight.x;
				}
			} else { // rhs didn't hit the side of lhs
				// hit TOP
				impulse.first.y = rhsPathEnd.y - lhsTopLeft.y;
			}
		} else if (rhsTransform->lastPos.y > rhsTransform->pos.y) { // rhs didn't hit the top of lhs
			const Vector2i lhsBottomLeft = {
				lhsTransform->pos.x + lhsCollider->GetAABB().x,
				lhsTransform->pos.y + lhsCollider->GetAABB().y + lhsCollider->GetAABB().h
			};
			const Vector2i lhsBottomRight = {
				lhsTransform->pos.x + lhsCollider->GetAABB().x + lhsCollider->GetAABB().w,
				lhsTransform->pos.y + lhsCollider->GetAABB().y + lhsCollider->GetAABB().h
			};
			if (rhsTransform->lastPos.x < rhsTransform->pos.x) { // rhs didn't hit the right of lhs
				if (UnsafeGetLineIntersectionX(rhsPathStart, rhsPathEnd, lhsBottomLeft, lhsBottomRight) >= lhsBottomLeft.x) {
					// hit BOTTOM
					impulse.first.y = rhsPathEnd.y - lhsBottomLeft.y;
				} else {
					// hit LEFT
					impulse.first.x = rhsPathEnd.x - lhsBottomLeft.x;
				}
			} else if (rhsTransform->lastPos.x > rhsTransform->pos.x) { // rhs didn't hit the left of lhs
				if (UnsafeGetLineIntersectionX(rhsPathStart, rhsPathEnd, lhsBottomLeft, lhsBottomRight) <= lhsBottomRight.x) {
					// hit BOTTOM
					impulse.first.y = rhsPathEnd.y - lhsBottomLeft.y;
				} else {
					// hit RIGHT
					impulse.first.x = rhsPathEnd.x - lhsBottomRight.x;
				}
			} else { // rhs didn't hit the side of lhs
				// hit BOTTOM
				impulse.first.y = rhsPathEnd.y - lhsBottomLeft.y;
			}
		} else { // rhs hit the side of lhs
			if (rhsTransform->lastPos.x < rhsTransform->pos.x) { // rhs didn't hit the right of lhs
				// hit LEFT
				impulse.first.x = rhsPathEnd.x - lhsTransform->pos.x - lhsCollider->GetAABB().x;
			} else if (rhsTransform->lastPos.x > rhsTransform->pos.x) { // rhs didn't hit the left of lhs
				// hit RIGHT
				impulse.first.x = rhsPathEnd.x - lhsTransform->pos.x - lhsCollider->GetAABB().x - lhsCollider->GetAABB().w;
			} else { // rhs actually didn't move
			}
		}
	} else if (rhsMomentum.x == 0 && rhsMomentum.y == 0) { // LHS PUSHES RHS COMPLETELY
		if (lhsTransform->lastPos.y < lhsTransform->pos.y) { // lhs didn't hit the bottom of rhs
			const Vector2i rhsTopLeft = rhsTransform->pos + rhsCollider->GetAABB().GetPos();
			const Vector2i rhsTopRight = {
				rhsTransform->pos.x + rhsCollider->GetAABB().x + rhsCollider->GetAABB().w,
				rhsTransform->pos.y + rhsCollider->GetAABB().y
			};
			if (lhsTransform->lastPos.x < lhsTransform->pos.x) { // lhs didn't hit the right of rhs
				if (UnsafeGetLineIntersectionX(lhsPathStart, lhsPathEnd, rhsTopLeft, rhsTopRight) >= rhsTopLeft.x) {
					// hit TOP
					impulse.second.y = lhsPathEnd.y - rhsTopLeft.y;
				} else {
					// hit LEFT
					impulse.second.x = lhsPathEnd.x - rhsTopLeft.x;
				}
			} else if (lhsTransform->lastPos.x > lhsTransform->pos.x) { // lhs didn't hit the left of rhs
				if (UnsafeGetLineIntersectionX(lhsPathStart, lhsPathEnd, rhsTopLeft, rhsTopRight) <= rhsTopRight.x) {
					// hit TOP
					impulse.second.y = lhsPathEnd.y - rhsTopLeft.y;
				} else {
					// hit RIGHT
					impulse.second.x = lhsPathEnd.x - rhsTopRight.x;
				}
			} else { // lhs didn't hit the side of rhs
				// hit TOP
				impulse.second.y = lhsPathEnd.y - rhsTopLeft.y;
			}
		} else if (lhsTransform->lastPos.y > lhsTransform->pos.y) { // lhs didn't hit the top of rhs
			const Vector2i rhsBottomLeft = {
				rhsTransform->pos.x + rhsCollider->GetAABB().x,
				rhsTransform->pos.y + rhsCollider->GetAABB().y + rhsCollider->GetAABB().h
			};
			const Vector2i rhsBottomRight = {
				rhsTransform->pos.x + rhsCollider->GetAABB().x + rhsCollider->GetAABB().w,
				rhsTransform->pos.y + rhsCollider->GetAABB().y + rhsCollider->GetAABB().h
			};
			if (lhsTransform->lastPos.x < lhsTransform->pos.x) { // lhs didn't hit the right of rhs
				if (UnsafeGetLineIntersectionX(lhsPathStart, lhsPathEnd, rhsBottomLeft, rhsBottomRight) >= rhsBottomLeft.x) {
					// hit BOTTOM
					impulse.second.y = lhsPathEnd.y - rhsBottomLeft.y;
				} else {
					// hit LEFT
					impulse.second.x = lhsPathEnd.x - rhsBottomLeft.x;
				}
			} else if (lhsTransform->lastPos.x > lhsTransform->pos.x) { // lhs didn't hit the left of rhs
				if (UnsafeGetLineIntersectionX(lhsPathStart, lhsPathEnd, rhsBottomLeft, rhsBottomRight) <= rhsBottomRight.x) {
					// hit BOTTOM
					impulse.second.y = lhsPathEnd.y - rhsBottomLeft.y;
				} else {
					// hit RIGHT
					impulse.second.x = lhsPathEnd.x - rhsBottomRight.x;
				}
			} else { // lhs didn't hit the side of rhs
				// hit BOTTOM
				impulse.second.y = lhsPathEnd.y - rhsBottomLeft.y;
			}
		} else { // lhs hit the side of rhs
			if (lhsTransform->lastPos.x < lhsTransform->pos.x) { // lhs didn't hit the right of rhs
				// hit LEFT
				impulse.second.x = lhsPathEnd.x - rhsTransform->pos.x - rhsCollider->GetAABB().x;
			} else if (lhsTransform->lastPos.x > lhsTransform->pos.x) { // lhs didn't hit the left of rhs
				// hit RIGHT
				impulse.second.x = lhsPathEnd.x - rhsTransform->pos.x - rhsCollider->GetAABB().x - rhsCollider->GetAABB().w;
			} else { // lhs actually didn't move
			}
		}
	} else { // lhs and rhs push eachother relative to their momentums

		// TODO: handle entities pushing eachother by combining thier momentums

		/*// calculate at what % of distance they collide
		Vector2f percentToCollide = {
			(float)(lhsPathStart.x - rhsPathStart.x) / (float)(rhsMomentum.x - lhsMomentum.x),
			(float)(lhsPathStart.y - rhsPathStart.y) / (float)(rhsMomentum.y - lhsMomentum.y)
		};
		Vector2f lhsLocalCollisionLocationX = {
			percentToCollide.x * lhsMomentum.x,
			percentToCollide.y * lhsMomentum.x
		};
		Vector2f rhsLocalCollisionLocationX = {
			percentToCollide.x * rhsMomentum.x,
			percentToCollide.y * rhsMomentum.x
		};

		const bool isHorizontalCollision = percentToCollide.x <= 1 && percentToCollide.x >= 0 && (int)std::round(lhsPathStart.x + lhsLocalCollisionLocationX.x) == (int)std::round(rhsPathStart.x + rhsLocalCollisionLocationX.x);
		// impulse depending upon the axis they're colliding in (up/down or left/right)
		if (isHorizontalCollision) {
			impulse.first.x = (int)std::round(-(1.f - percentToCollide.x) * lhsMomentum.x);
			impulse.second.x = lhsPathEnd.x + impulse.first.x - rhsPathEnd.x;
		} else {
			impulse.first.y = (int)std::round(-(1.f - percentToCollide.y) * lhsMomentum.y);
			impulse.second.y = lhsPathEnd.y + impulse.first.y - rhsPathEnd.y;
		}*/
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