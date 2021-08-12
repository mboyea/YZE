#include "ScriptSystem.h"

void Systems::Scripts::Update(ScriptList* scripts, Entities::Index entity) {
	for (Script* script : scripts->scripts) {
		script->Update(entity);
	}
}