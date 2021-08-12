#ifndef SCRIPT_SYSTEM_H
#define SCRIPT_SYSTEM_H
#pragma once

#include "ScriptList.h"

namespace Systems {
	namespace Scripts {
		void Update(ScriptList* scripts, Entities::Index entity);
	}
}

#endif // !SCRIPT_SYSTEM_H