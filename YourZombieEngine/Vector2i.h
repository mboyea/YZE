#ifndef VECTOR2I_H
#define VECTOR2I_H
#pragma once

struct Vector2i {
	int x;
	int y;
	Vector2i operator+(const Vector2i& rhs) const {
		return { x + rhs.x, y + rhs.y };
	}
	Vector2i operator-(const Vector2i& rhs) const {
		return { x - rhs.x, y - rhs.y };
	}
	Vector2i& operator+=(const Vector2i& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	Vector2i& operator-=(const Vector2i& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
};

#endif // !VECTOR2I_H