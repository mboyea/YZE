#ifndef VECTOR2I_H
#define VECTOR2I_H
#pragma once

struct Vector2i {
	int x;
	int y;

	inline Vector2i operator+(const Vector2i& rhs) {
		return { x + rhs.x, y + rhs.y };
	}
	Vector2i operator-(const Vector2i& rhs) {
		return { x - rhs.x, y - rhs.y };
	}
};

#endif // !VECTOR2I_H