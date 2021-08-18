#ifndef VECTOR4I_H
#define VECTOR4I_H
#pragma once

#include "Vector2i.h"

struct Vector4i {
	int x;
	int y;
	int w;
	int h;
	const Vector2i GetPos() const {
		return { x, y };
	}
	const Vector2i GetDim() const {
		return { w, h };
	}
};

#endif // !VECTOR4I_H