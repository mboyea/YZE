#ifndef SCRIPT_H
#define SCRIPT_H
#pragma once

#include "Component.h"
#include <vector>

namespace Scripts {
	typedef int ID;
	const ID INVALID_TYPE = -1;
}

class Script : public Component {
	INIT_COMPONENT();
private:
	static Scripts::ID scriptID;
public:

	virtual Scripts::ID GetScriptID() {
		return Scripts::INVALID_TYPE;
	}
	virtual std::ostream& Serialize(std::ostream& os) override;
	virtual std::istream& Deserialize(std::istream& is) override;
};

namespace Scripts {
	// TOOD: mimic Component design to store script functions
	// possibly use macros and #if statements to check if a function exists and then add it to a void* list specific
	// to that script ID.  this list may make its indexes reflective of specific script types.  If so, I will have to
	// find some way to compress these lists.  Maybe make indexes relflective of script ids and sub-indexes reflective
	// of script types.  handle the calling of these scripts in ScriptSystem.h
}

#define INIT_SCRIPT() 

#define REGISTER_SCRIPT(c) 

#endif // !SCRIPT_H