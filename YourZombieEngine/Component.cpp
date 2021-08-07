#include "Component.h"

std::set<Resource>& Component::GetRequiredResources(std::set<Resource>& resourcesOut) {
	return resourcesOut;
}

std::ostream& Component::Serialize(std::ostream& os) {
	return os << GetVirtualTypeID();
}

std::istream& Component::Deserialize(std::istream& is) {
	// The ID is intended to be taken by an external interpreter and passed into the Components::IDToType()
	// function to create the source object that Deserialize is run on. Therefore, the component itself
	// expects this ID to be gone when deserializing from the line. If the istream does not have this ID
	// taken out of it, it WILL incorrectly load the entity data.
	return is;
}
