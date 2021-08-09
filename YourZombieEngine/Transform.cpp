#include "Transform.h"
REGISTER_COMPONENT(Transform);

Transform::Transform()
	: pos({ 0, 0 }) {}

std::ostream& Transform::Serialize(std::ostream& os) {
	// TODO: logic
	return os;
}

std::istream& Transform::Deserialize(std::istream& is) {
	// TODO: logic
	return is;
}
