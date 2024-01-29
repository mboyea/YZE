#ifndef VECTOR2F_H
#define VECTOR2F_H
#pragma once

struct Vector2f {
	float x;
	float y;
	Vector2f operator+(const Vector2f& rhs) const {
		return { x + rhs.x, y + rhs.y };
	}
	Vector2f operator-(const Vector2f& rhs) const {
		return { x - rhs.x, y - rhs.y };
	}
	Vector2f& operator+=(const Vector2f& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	Vector2f& operator-=(const Vector2f& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
};

#endif // !VECTOR2F_H