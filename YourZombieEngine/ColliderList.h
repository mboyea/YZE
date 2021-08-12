#ifndef COLLIDER_LIST_H
#define COLLIDER_LIST_H
#pragma once

#include "Component.h"
#include "Collider.h"
#include <vector>

class ColliderList : public Component {
	INIT_COMPONENT;
public:
	std::vector<Collider*> colliders;
	virtual std::ostream& Serialize(std::ostream& os) override;
	virtual std::istream& Deserialize(std::istream& is) override;
};

#endif // !COLLIDER_LIST_H