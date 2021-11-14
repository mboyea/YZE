#include "ColliderList.h"
REGISTER_COMPONENT(ColliderList);
#include "Files.h"

std::ostream& ColliderList::Serialize(std::ostream& os) {
	Component::Serialize(os);
	for (Collider* collider : colliders) {
		os << ' ';
		collider->Serialize(os);
	}
	return os << ' ' << Files::ARRAY_END;
}

std::istream& ColliderList::Deserialize(std::istream& is) {
	// peek the first character of the first element of the array
	is >> std::ws;
	char peek = is.peek();
	// while this character is valid and isn't a negative number
	while (peek != Files::ARRAY_END && peek != EOF && peek != '-') {
		// determine the collider type
		Colliders::TypeID type;
		is >> type;
		// generate the new collider
		Collider* collider = Colliders::typeArray[type]();
		// deserialize the new collider from the stream according to its type (via polymorphism)
		collider->Deserialize(is);
		// add the the new collider to the collider list
		colliders.push_back(collider);
		// peek the first character of the next element of the array
		is >> std::ws;
		char peek = is.peek();
	}
	return is;
}