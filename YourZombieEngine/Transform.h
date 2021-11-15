#ifndef TRANSFORM_H
#define TRANSFORM_H
#pragma once

#include "Component.h"
#include "Vector2i.h"

class Transform : public Component {
	INIT_COMPONENT;
public:
	Vector2i lastPos;
	Vector2i pos;
	Transform();
	virtual ~Transform() override;
	virtual std::ostream& Serialize(std::ostream& os) override;
	virtual std::istream& Deserialize(std::istream& is) override;
};

#endif // !TRANSFORM_H