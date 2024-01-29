#include "Script.h"
const std::string Script::name = "Script";

std::set<Resource>& Script::GetRequiredResources(std::set<Resource>& resourcesOut) {
	return resourcesOut;
}

std::ostream& Script::Serialize(std::ostream& os) {
	return os << VirtualGetTypeID();
}

std::istream& Script::Deserialize(std::istream& is) {
	return is;
}
