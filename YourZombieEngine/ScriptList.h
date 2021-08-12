#ifndef SCRIPT_LIST_H
#define SCRIPT_LIST_H
#pragma once

#include "Component.h"
#include "Script.h"
#include <vector>

class ScriptList : public Component {
	INIT_COMPONENT;
public:
	std::vector<Script*> scripts;
	virtual std::ostream& Serialize(std::ostream& os) override;
	virtual std::istream& Deserialize(std::istream& is) override;
};

#endif // !SCRIPT_LIST_H