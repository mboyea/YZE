#ifndef TRANSFORM_H
#define TRANSFORM_H
#pragma once

#include "Component.h"
#include "Vector2i.h"

class Transform : public Component {
	INIT_COMPONENT();
public:
	Vector2i pos;
};

#endif // !TRANSFORM_H