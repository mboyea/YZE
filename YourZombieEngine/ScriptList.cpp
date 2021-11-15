#include "ScriptList.h"
REGISTER_COMPONENT(ScriptList);
#include "Files.h"

ScriptList::~ScriptList() {
	for (Script* collider : scripts) {
		delete collider;
	}
}

std::set<Resource>& ScriptList::GetRequiredResources(std::set<Resource>& resourcesOut) {
	Component::GetRequiredResources(resourcesOut);
	for (Script* script : scripts) {
		script->GetRequiredResources(resourcesOut);
	}
	return resourcesOut;
}

std::ostream& ScriptList::Serialize(std::ostream& os) {
	Component::Serialize(os);
	for (Script* script : scripts) {
		os << ' ';
		script->Serialize(os);
	}
	return os << ' ' << Files::ARRAY_END;
}

std::istream& ScriptList::Deserialize(std::istream& is) {
	// peek the first character of the first element of the array
	is >> std::ws;
	char peek = is.peek();
	// while this character is valid
	while (peek != Files::ARRAY_END && peek != EOF) {
		// determine the script type
		Scripts::TypeID type;
		is >> type;
		// generate the new script
		Script* script = Scripts::typeArray[type]();
		// deserialize the new script from the stream according to its type (via polymorphism)
		script->Deserialize(is);
		// add the the new script to the script list
		scripts.push_back(script);
		// peek the first character of the next element of the array
		is >> std::ws;
		char peek = is.peek();
	}
	return is;
}