#ifndef COMPONENT_H
#define COMPONENT_H
#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "Resource.h"
#include "Entities.h"

namespace Components {
	typedef int TypeID;
	const TypeID INVALID_TYPE = -1;
}

class Component {
private:
	static const std::string name;
public:
	Entities::Index entity;
	virtual ~Component();
	virtual Components::TypeID GetVirtualTypeID() {
		return Components::INVALID_TYPE;
	}
	static constexpr Components::TypeID GetTypeID() {
		return Components::INVALID_TYPE;
	}
	virtual std::string GetVirtualName() {
		return name;
	}
	static std::string GetName() {
		return name;
	}
	virtual std::set<Resource>& GetRequiredResources(std::set<Resource>& resourcesOut);
	virtual std::ostream& Serialize(std::ostream& os);
	virtual std::istream& Deserialize(std::istream& is);
};

namespace Components {
	inline std::vector<Component*(*)()> typeArray;
	template<typename T> class Register {
	public:
		const Components::TypeID typeID;
		Register() : typeID(typeArray.size()) {
			typeArray.push_back([]()->Component* { return new T(); });
		}
	};
	template<typename T> TypeID TypeToID() {
		return T::GetTypeID();
	}
	inline Component* IDToType(TypeID id) {
		return typeArray[id]();
	}
	inline TypeID GetTypeCount() {
		return typeArray.size();
	}
}

#define INIT_COMPONENT public:	\
virtual Components::TypeID GetVirtualTypeID() override {	\
return typeID;	\
}	\
static constexpr Components::TypeID GetTypeID() {	\
return typeID;	\
}	\
virtual std::string GetVirtualName() override {	\
return name;	\
}	\
static std::string GetName() {	\
return name;	\
}	\
private:	\
static const Components::TypeID typeID;	\
static const std::string name

#define REGISTER_COMPONENT(c) static Components::Register<c> Registered_##c;	\
const Components::TypeID c::typeID = Registered_##c.typeID;	\
const std::string c::name = #c

#endif // !COMPONENT_H
