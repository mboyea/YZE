#ifndef PLAYER_SCRIPT_H
#define PLAYER_SCRIPT_H

#include "Script.h"
#include "Transform.h"

class PlayerScript : public Script {
	INIT_SCRIPT;
public:
	Transform* transform;
	virtual void Start(Entities::Index entity) override;
	virtual void Update(Entities::Index entity) override;
	virtual void OnCollision(Entities::Index entity, Entities::Index rhs, Vector2i impulse) override;
};

#endif // !PLAYER_SCRIPT_H