#include "Component.h"

std::vector<Component* (*)()> components;

template<typename DerivedType> Components::Register<DerivedType>::Register() {
	components.push_back([]()->Component* { return new DerivedType(); });
}