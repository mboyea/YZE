#ifndef COMPONENT_H
#define COMPONENT_H
#pragma once

#include <vector>
#include <iostream>
#include "Resource.h"

namespace Components {
	typedef int ID;
	const ID INVALID_TYPE = -1;
}

class Component {
public:
	virtual Components::ID GetVirtualTypeID() {
		return Components::INVALID_TYPE;
	}
	static Components::ID GetTypeID() {
		return Components::INVALID_TYPE;
	}
	virtual std::set<Resource>& GetRequiredResources(std::set<Resource>& resourcesOut);
	virtual std::ostream& Serialize(std::ostream& os);
	virtual std::istream& Deserialize(std::istream& is);
};

namespace Components {
	inline std::vector<Component* (*)()> typeArray;
	template<typename T> class Register {
	public:
		const Components::ID typeID;
		Register() : typeID(typeArray.size()) {
			typeArray.push_back([]()->Component* { return new T(); });
		}
	};
	template<typename T> ID TypeToID() {
		return T::GetTypeID();
	}
	inline Component* IDToType(ID id) {
		return typeArray[id]();
	}
	inline size_t GetTypeCount() {
		return typeArray.size();
	}
}

#define INIT_COMPONENT() public:						\
virtual Components::ID GetVirtualTypeID() override {	\
return typeID;											\
}														\
static Components::ID GetTypeID() {						\
return typeID;											\
}														\
private:												\
static const Components::ID typeID;

#define REGISTER_COMPONENT(c) static Components::Register<##c> Registered_##c;	\
const Components::ID c::typeID = Registered_##c.typeID;

#endif // !COMPONENT_H