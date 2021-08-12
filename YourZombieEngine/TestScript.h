#ifndef TEST_SCRIPT_H
#define TEST_SCRIPT_H
#pragma once

#include "Script.h"

class TestScript : public Script {
	INIT_SCRIPT;
public:
	virtual void Update(Entities::Index entity) override;
	virtual void OnCollision(Collision* collision, Entities::Index entity) override;
};

#endif // !TEST_SCRIPT_H