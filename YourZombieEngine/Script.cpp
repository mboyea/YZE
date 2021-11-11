#include "Script.h"
const std::string Script::name = "Script";

std::set<Resource>& Script::GetRequiredResources(std::set<Resource>& resourcesOut) {
	return resourcesOut;
}
