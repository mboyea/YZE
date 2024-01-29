#ifndef TEST_SCRIPT_H
#define TEST_SCRIPT_H
#pragma once

#include "Script.h"
#include "Transform.h"

class TestScript : public Script {
	INIT_SCRIPT;
public:
	Transform* transform;
	virtual void Start(Entities::Index entity) override;
	virtual void Update(Entities::Index entity) override;
	virtual void OnCollision(Entities::Index entity, Entities::Index rhs, Vector2i impulse) override;
};

#endif // !TEST_SCRIPT_H