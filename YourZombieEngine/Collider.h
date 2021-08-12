#ifndef COLLIDER_H
#define COLLIDER_H
#pragma once

#include <vector>
#include "Vector2i.h"

namespace Colliders {
	typedef int TypeID;
	const TypeID INVALID_TYPE = -1;
}

class Collider {
public:
	static constexpr Colliders::TypeID GetColliderID() {
		return Colliders::INVALID_TYPE;
	}
	virtual const Colliders::TypeID VirtualGetColliderID() {
		return GetColliderID();
	}
	static constexpr const char* ToString() {
		return str;
	}
	virtual const char* VirtualToString() {
		return ToString();
	}
private:
	static const char str[];
private:
public:
	Collider();
	~Collider();
};

namespace Colliders {
	inline std::vector<Collider*(*)()> typeArray;
	template<typename T> class Register {
	public:
		const Colliders::TypeID typeID;
		Register() : typeID(typeArray.size()) {
			typeArray.push_back([]()->Collider* { return new T(); });
		}
	};
	inline Collider* New(TypeID id) {
		return typeArray[id]();
	}
	template<typename T> inline T* New() {
		return static_cast<T*>(NewCollider(T::GetColliderID()));
	}
	template<typename T> inline TypeID GetTypeID() {
		return T::GetColliderID();
	}
	inline TypeID GetTypeCount() {
		return typeArray.size();
	}
}

#define INIT_COLLIDER public:	\
static constexpr Colliders::TypeID GetColliderID() {	\
return typeID;	\
}	\
virtual const Colliders::TypeID VirtualGetColliderID() override {	\
return GetColliderID();	\
}	\
static constexpr const char* ToString() {	\
return str;	\
}	\
virtual const char* VirtualToString() override {	\
return ToString();	\
}	\
private:	\
static const Colliders::TypeID typeID;	\
static const char str[]

#define REGISTER_COLLIDER(c) static Colliders::Register<##c> Registered_##c;	\
const Colliders::TypeID c::typeID = Registered_##c.typeID;	\
const char c::str[] = #c

#endif // !COLLIDER_H