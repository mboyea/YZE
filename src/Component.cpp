#include "Component.h"
const std::string Component::name = "Component";

Component::~Component() {}

std::set<Resource>& Component::GetRequiredResources(std::set<Resource>& resourcesOut) {
	return resourcesOut;
}

std::ostream& Component::Serialize(std::ostream& os) {
	return os << GetVirtualTypeID();
}

std::istream& Component::Deserialize(std::istream& is) {
	// NOTE: The ID is intended to be taken by an external interpreter and passed into the
	// Components::IDToType() function to create the source object that Deserialize is run
	// on. Therefore, the component itself expects this ID to already be gone when
	// deserializing.
	return is;
}