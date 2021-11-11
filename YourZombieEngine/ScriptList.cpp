#include "ScriptList.h"
REGISTER_COMPONENT(ScriptList);

std::set<Resource>& ScriptList::GetRequiredResources(std::set<Resource>& resourcesOut) {
	Component::GetRequiredResources(resourcesOut);
	for (Script* script : scripts) {
		script->GetRequiredResources(resourcesOut);
	}
	return resourcesOut;
}

std::ostream& ScriptList::Serialize(std::ostream& os) {
	// TODO: logic
	return os;
}

std::istream& ScriptList::Deserialize(std::istream& is) {
	// TODO: logic
	return is;
}