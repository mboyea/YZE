#ifndef VECTOR2I_H
#define VECTOR2I_H
#pragma once

#include <iostream>

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
	friend std::ostream& operator<<(std::ostream& os, const Vector2i& rhs) {
		return os << rhs.x << ' ' << rhs.y;
	}
	friend std::istream& operator>>(std::istream& is, Vector2i& rhs) {
		return is >> rhs.x >> rhs.y;
	}
};

#endif // !VECTOR2I_H