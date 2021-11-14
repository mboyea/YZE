#include "Transform.h"
REGISTER_COMPONENT(Transform);

Transform::Transform()
	: pos({ 0, 0 }), lastPos({ 0, 0 }) {}

std::ostream& Transform::Serialize(std::ostream& os) {
	return Component::Serialize(os) << ' ' << pos << ' ' << lastPos;
}

std::istream& Transform::Deserialize(std::istream& is) {
	return is >> pos >> lastPos;
}