#ifndef VECTOR4I_H
#define VECTOR4I_H
#pragma once

#include "Vector2i.h"

struct Vector4i {
	int x;
	int y;
	int w;
	int h;
	Vector2i GetPos() {
		return { x, y };
	}
	Vector2i GetDim() {
		return { w, h };
	}
};

#endif // !VECTOR4I_H